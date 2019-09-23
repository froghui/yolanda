#include "http_response.h"
#include "common.h"

#define INIT_RESPONSE_HEADER_SIZE 128

struct http_response *http_response_new() {
    struct http_response *httpResponse = malloc(sizeof(struct http_response));
    httpResponse->body = NULL;
    httpResponse->statusCode = Unknown;
    httpResponse->statusMessage = NULL;
    httpResponse->response_headers = malloc(sizeof(struct response_header) * INIT_RESPONSE_HEADER_SIZE);
    httpResponse->response_headers_number = 0;
    httpResponse->keep_connected = 0;
    return httpResponse;
}

void http_response_encode_buffer(struct http_response *httpResponse, struct buffer *output) {
    char buf[32];
    snprintf(buf, sizeof buf, "HTTP/1.1 %d ", httpResponse->statusCode);
    buffer_append_string(output, buf);
    buffer_append_string(output, httpResponse->statusMessage);
    buffer_append_string(output, "\r\n");

    if (httpResponse->keep_connected) {
        buffer_append_string(output, "Connection: close\r\n");
    } else {
        snprintf(buf, sizeof buf, "Content-Length: %zd\r\n", strlen(httpResponse->body));
        buffer_append_string(output, buf);
        buffer_append_string(output, "Connection: Keep-Alive\r\n");
    }

    if (httpResponse->response_headers != NULL && httpResponse->response_headers_number > 0) {
        for (int i = 0; i < httpResponse->response_headers_number; i++) {
            buffer_append_string(output, httpResponse->response_headers[i].key);
            buffer_append_string(output, ": ");
            buffer_append_string(output, httpResponse->response_headers[i].value);
            buffer_append_string(output, "\r\n");
        }
    }

    buffer_append_string(output, "\r\n");
    buffer_append_string(output, httpResponse->body);
}