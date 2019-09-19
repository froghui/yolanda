#include "lib/http_request.h"
#include "lib/http_server.h"
#include "lib/http_response.h"

char *data = "GET / HTTP/1.1\r\nHost: localhost:43211\r\nUser-Agent: curl/7.54.0\r\nAccept: */*\r\n\r\n";


//数据读到buffer之后的callback
int onRequest(struct http_request *httpRequest, struct http_response *httpResponse) {
    char *url = httpRequest->url;
    char *question = memmem(url, strlen(url), "?", 1);
    char *path = NULL;
    if (question != NULL) {
        path = malloc(question - url);
        strncpy(path, url, question - url);
    } else {
        path = malloc(strlen(url));
        strncpy(path, url, strlen(url));
    }

    if (strcmp(path, "/") == 0) {
        httpResponse->statusCode = OK;
        httpResponse->statusMessage = "OK";
        httpResponse->contentType = "text/html";
        httpResponse->body = "<html><head><title>This is network programming</title></head><body><h1>Hello, network programing</h1></body></html>";
    } else if (strcmp(path, "network") == 0) {
        httpResponse->statusCode = OK;
        httpResponse->statusMessage = "OK";
        httpResponse->contentType = "text/plain";
        httpResponse->body = "hello, network programming";
    } else {
        httpResponse->statusCode = NotFound;
        httpResponse->statusMessage = "Not Found";
        httpResponse->keep_connected = 1;
    }

    return 0;
}


int main() {
    struct buffer *input = buffer_new();
    buffer_append_string(input, data);

    struct http_request *httpRequest = http_request_new();
    parse_http_request(input, httpRequest);

    for (int i = 0; i < httpRequest->request_headers_number; i++) {
        char *key = httpRequest->request_headers[i].key;
        char *value = httpRequest->request_headers[i].value;
        printf("key == %s, value == %s\n", key, value);
    }

    struct http_response *httpResponse = http_response_new();
    onRequest(httpRequest, httpResponse);

    struct buffer *output = buffer_new();
    http_response_encode_buffer(httpResponse, output);
}