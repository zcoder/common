/* 
 * File:   http_server.c++
 * Author: zhenka
 * 
 * Created on 9 Июнь 2012 г., 6:07
 */

#include "http_server.h"
#include "c_evbuffer.h"
http_server::http_server( const in_addr_t in_addr, const in_port_t in_port, const uint32_t in_thread_count )
{
    struct sockaddr_in l_sockaddr_in;
    memset(&l_sockaddr_in, 0, sizeof(l_sockaddr_in));
    l_sockaddr_in.sin_family=AF_INET;
    l_sockaddr_in.sin_addr.s_addr=in_addr;
    l_sockaddr_in.sin_port=in_port;
    int m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) 
        throw std::exception();
    int value = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&value, sizeof(int));
    int result = bind(m_socket, (struct sockaddr*)&l_sockaddr_in, sizeof(l_sockaddr_in));
    if (result < 0) 
        throw std::exception();
    result = listen(m_socket, 32000);
    if (result < 0)
        throw std::exception();
    http_server(m_socket, in_thread_count);
}

http_server::http_server( int socket, const uint32_t in_thread_count ):
m_socket(socket)
{
    int flags;
    int result;
    if ((flags = fcntl(m_socket, F_GETFL, 0)) < 0 || fcntl(m_socket, F_SETFL, flags | O_NONBLOCK) < 0)
        throw std::exception();
    fill_mime_type();
    
    // --- signal magic
    struct sigaction sigact_pipe;
    sigset_t sigset_pipe;
    sigemptyset(&sigset_pipe);
    sigaddset(&sigset_pipe, SIGPIPE);
    memset(&sigact_pipe, 0, sizeof(sigact_pipe));
    sigact_pipe.sa_mask = sigset_pipe;
    sigact_pipe.sa_handler=SIG_IGN;
    sigact_pipe.sa_flags = 0;
    sigaction(SIGPIPE, &sigact_pipe, NULL);
    sigprocmask(SIG_BLOCK, &sigset_pipe, NULL);
    // --- signal magic
    
    pthread_t threads[in_thread_count];
    for (int i=0; i < in_thread_count; ++i)
    {
        struct event_base *ev_base = event_init();
        if (ev_base == NULL) 
            throw std::exception();
        struct evhttp *ev_httpd = evhttp_new(ev_base);
        if (ev_httpd == NULL)
            throw std::exception();
        result = evhttp_accept_socket(ev_httpd, m_socket);
        if (result != 0)
            throw std::exception();
        evhttp_set_timeout(ev_httpd, 2);
        evhttp_set_gencb(ev_httpd, generic_callback, this);
        result = pthread_create(&threads[i], NULL, dispatch, ev_base);
        if (result != 0)
            throw std::exception();
    }
    for (int i = 0; i < in_thread_count; ++i) 
    {
        pthread_join(threads[i], NULL);
    }
}

http_server::~http_server( )
{
    std::cerr << __PRETTY_FUNCTION__ << std::endl;
}

std::string http_server::find_mime_type(std::string in_ext)
{
    mime_map_t::const_iterator it=m_mime_map.find(in_ext);
    if (it != m_mime_map.end())
        return it->second;
    return "application/misc";
}

void http_server::fill_mime_type()
{
    m_mime_map.clear();
    //sorry hardcode )
    m_mime_map.insert(mime_map_t::value_type("gif","image/gif"));
    m_mime_map.insert(mime_map_t::value_type("html","text/html"));
    m_mime_map.insert(mime_map_t::value_type("htm","text/html"));
    m_mime_map.insert(mime_map_t::value_type("jpeg","image/jpeg"));
    m_mime_map.insert(mime_map_t::value_type("jpg","image/jpeg"));
    m_mime_map.insert(mime_map_t::value_type("jpe","image/jpeg"));
    m_mime_map.insert(mime_map_t::value_type("css","text/css"));
    m_mime_map.insert(mime_map_t::value_type("png","image/png"));
    m_mime_map.insert(mime_map_t::value_type("bmp","image/bmp"));
    m_mime_map.insert(mime_map_t::value_type("cgm","image/cgm"));
    m_mime_map.insert(mime_map_t::value_type("ief","image/ief"));
    m_mime_map.insert(mime_map_t::value_type("svg","image/svg+xml"));
    m_mime_map.insert(mime_map_t::value_type("tiff","image/tiff"));
    m_mime_map.insert(mime_map_t::value_type("tif","image/tiff"));
    m_mime_map.insert(mime_map_t::value_type("wbmp","image/vnd.wap.wbmp"));
    m_mime_map.insert(mime_map_t::value_type("ras","image/x-cmu-raster"));
    m_mime_map.insert(mime_map_t::value_type("ico","image/x-icon"));
    m_mime_map.insert(mime_map_t::value_type("pnm","image/x-portable-anymap"));
    m_mime_map.insert(mime_map_t::value_type("pbm","image/x-portable-bitmap"));
    m_mime_map.insert(mime_map_t::value_type("pgm","image/x-portable-graymap"));
    m_mime_map.insert(mime_map_t::value_type("ppm","image/x-portable-pixmap"));
    m_mime_map.insert(mime_map_t::value_type("rgb","image/x-rgb"));
    m_mime_map.insert(mime_map_t::value_type("xbm","image/x-xbitmap"));
    m_mime_map.insert(mime_map_t::value_type("xpm","image/x-xpixmap"));
    m_mime_map.insert(mime_map_t::value_type("xwd","image/x-xwindowdump"));
    m_mime_map.insert(mime_map_t::value_type("js","application/x-javascript"));
    m_mime_map.insert(mime_map_t::value_type("swf","application/x-shockwave-flash"));
    m_mime_map.insert(mime_map_t::value_type("xhtml","pplication/xhtml+xml"));
    m_mime_map.insert(mime_map_t::value_type("xht","pplication/xhtml+xml"));
    m_mime_map.insert(mime_map_t::value_type("xslt","application/xslt+xml"));
    m_mime_map.insert(mime_map_t::value_type("xml","application/xml"));
    m_mime_map.insert(mime_map_t::value_type("xsl","application/xml"));
    m_mime_map.insert(mime_map_t::value_type("dtd","application/xml-dtd"));
    m_mime_map.insert(mime_map_t::value_type("mid","audio/midi"));
    m_mime_map.insert(mime_map_t::value_type("midi","audio/midi"));
    m_mime_map.insert(mime_map_t::value_type("kar","audio/midi"));
    m_mime_map.insert(mime_map_t::value_type("mpga","audio/mpeg"));
    m_mime_map.insert(mime_map_t::value_type("mp2","audio/mpeg"));
    m_mime_map.insert(mime_map_t::value_type("mp3","audio/mpeg"));
    m_mime_map.insert(mime_map_t::value_type("wrl","model/vrml"));
    m_mime_map.insert(mime_map_t::value_type("vrml","model/vrml"));
    m_mime_map.insert(mime_map_t::value_type("asc","text/plain"));
    m_mime_map.insert(mime_map_t::value_type("txt","text/plain"));
    m_mime_map.insert(mime_map_t::value_type("rtx","text/richtext"));
    m_mime_map.insert(mime_map_t::value_type("rtf","text/rtf"));
    m_mime_map.insert(mime_map_t::value_type("sgm","text/sgml"));
    m_mime_map.insert(mime_map_t::value_type("sgml","text/sgml"));
    m_mime_map.insert(mime_map_t::value_type("tsv","text/tab-separated-values"));
    m_mime_map.insert(mime_map_t::value_type("wml","text/vnd.wap.wml"));
    m_mime_map.insert(mime_map_t::value_type("wmls","text/vnd.wap.wmlscript"));
    m_mime_map.insert(mime_map_t::value_type("etx","text/x-setext"));
    m_mime_map.insert(mime_map_t::value_type("mpeg","video/mpeg"));
    m_mime_map.insert(mime_map_t::value_type("mpg","video/mpeg"));
    m_mime_map.insert(mime_map_t::value_type("mpe","video/mpeg"));
    m_mime_map.insert(mime_map_t::value_type("qt","video/quicktime"));
    m_mime_map.insert(mime_map_t::value_type("mov","video/quicktime"));
    m_mime_map.insert(mime_map_t::value_type("mxu","video/vnd.mpegurl"));
    m_mime_map.insert(mime_map_t::value_type("m4u","video/vnd.mpegurl"));
    m_mime_map.insert(mime_map_t::value_type("avi","video/x-msvideo"));
    m_mime_map.insert(mime_map_t::value_type("movie","video/x-sgi-movie"));
    m_mime_map.insert(mime_map_t::value_type("flv","video/x-flv"));
    m_mime_map.insert(mime_map_t::value_type("mp4","audio/mp4"));
}

void http_server::process_request(struct evhttp_request* in_request)
{
    try
    {
        c_evbuffer l_buffer;
        //c_evrequest in_request(in_request_ptr);
        // ---- prepare
        const char* l_uri = evhttp_request_uri(in_request);
        if (!strcmp(l_uri, "/exit"))
            exit(2);
        ++cb_count;
        struct timeval curr;
        gettimeofday(&curr, NULL);
        double timediff = ((double)curr.tv_sec + (double)curr.tv_usec/1000000) - ((double)start.tv_sec + (double)start.tv_usec/1000000);
        int fd;
        struct stat stbuf; 
        int total_read_bytes, read_bytes;
        // ---- processing
        if (!l_uri)
        {
            evbuffer_add_printf(l_buffer, "Bad request"); 
            evhttp_send_reply(in_request, HTTP_BADREQUEST, "Bad request", l_buffer); 
            return; 
        }

        if ((fd = open (l_uri, O_RDONLY)) < 0) 
        { 
            evbuffer_add_printf(l_buffer, " File %s not found", l_uri); 
            evhttp_send_reply(in_request, HTTP_NOTFOUND, "File not found", l_buffer); 
            return; 
        } 
        if (fstat (fd, &stbuf) < 0) 
        { 
            evbuffer_add_printf(l_buffer, "File %s not found", l_uri); 
            evhttp_send_reply(in_request, HTTP_NOTFOUND, "File not found", l_buffer); 
            close(fd); 
            return; 
        } 

        total_read_bytes = 0; 
        while (total_read_bytes < stbuf.st_size) 
        { 
            read_bytes = evbuffer_read(l_buffer, fd, stbuf.st_size); 
            if (read_bytes < 0) 
            { 
                evbuffer_add_printf(l_buffer, "Error reading file %s", l_uri); 
                evhttp_send_reply(in_request, HTTP_NOTFOUND, "File not found", l_buffer); 
                close(fd);
                return;
            } 
            total_read_bytes += read_bytes; 
        } 

        std::string fn = l_uri;
        std::string fe = fn.substr(fn.find_last_of(".") + 1);
        std::string mime = find_mime_type(fe);

        evhttp_add_header (in_request->output_headers, "Content-Type", mime.c_str());
        evhttp_send_reply (in_request, HTTP_OK, "HTTP_CONTENT_OK", l_buffer);

        close(fd);

        // ---- post_processing
        if (!(cb_count % 10000))
        {
            std::cerr << "req=" << cb_count << " s=" << timediff << " rps=" << double(double(cb_count) / timediff) << " uri=" << l_uri << std::endl;
        }
    }
    catch(...)
    {
        if (in_request)
            evhttp_request_free(in_request);
        std::cerr << "exception" << std::endl;
    }
}

void* dispatch(void* in_arg)
{
    event_base_dispatch((struct event_base*)in_arg);
    return NULL;
}

void generic_callback(struct evhttp_request* in_request, void* in_arg)
{
    ((http_server*)in_arg)->process_request(in_request);
}
