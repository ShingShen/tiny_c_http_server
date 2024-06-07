#include "request.h"

void send_data(FILE *fp, char *ct, char *file_name)
{
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char cnt_len[] = "Content-Length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;

    sprintf(cnt_type, "Content-Type:%s\r\n\r\n", ct);

    char file_path[SMALL_BUF];
    sprintf(file_path, "templates/%s", file_name);
    send_file = fopen(file_path, "r");

    if (send_file == NULL) {
        send_error(fp);
        return;
    }

    fputs(protocol, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    while (fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }

    fflush(fp);
    fclose(fp);
}

void send_error(FILE *fp)
{
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-Length:2048\r\n";
    char cnt_type[] = "Content-Type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>Error! Please check the file name and request method!"
                     "</font></body></html>";

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
}

char *content_type(char *file)
{
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    
    strcpy(file_name, file);
    if (!strchr(file_name, '.')) {
        return "text/plain";
    }

    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if (strcmp(extension, "html") == 0 || strcmp(extension, "htm") == 0) {
        return "text/html";
    } else if (strcmp(extension, "txt") == 0) {
        return "text/plain";
    } else if (strcmp(extension, "css") == 0) {
        return "text/css";
    } else if (strcmp(extension, "js") == 0) {
        return "text/javascript";
    } else if (strcmp(extension, "jpg") == 0 || strcmp(extension, "jpeg") == 0) {
        return "image/jpeg";
    } else if (strcmp(extension, "png") == 0) {
        return "image/png";
    } else if (strcmp(extension, "gif") == 0) {
        return "image/gif";
    } else {
        return "text/plain";
    }
}