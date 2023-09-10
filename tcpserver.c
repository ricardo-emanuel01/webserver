# include "common.h"
# include "routes.h"


int main(int argc, char **argv) {
    int                 listenfd, connfd, n;
    struct sockaddr_in  servAddr;
    uint8_t             buff[MAXLINE + 1];
    uint8_t             tempBuff[MAXLINE + 1];
    uint8_t             recoveryLine[MAXLINE + 1];
    char                endpoint[MAXLINE];
    char                key1[] = "abcde";
    char                template1[] = "<!DOCTYPE html><html lang=\"pt-BR\"><head><title>HelloWorld</title></head><body><h1>Ricardo Emanuel de Oliveira</h1></body></html>";
    char                key2[] = "home";
    char                template2[] = "<head><title>Home Page</title></head><body><h1>Welcome, to the home page</h1></body>";
    char                key3[] = "batman";
    char                template3[] = "<head><title>I am Batman</title></head><body><h1>E aí, papis</h1><h2>É nóis</h2></body>";
    NodePtr             routes = NULL;

    routes = addRoute(routes, key1, template1);
    routes = addRoute(routes, key2, template2);
    routes = addRoute(routes, key3, template3);

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_n_die("socket error.");

    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family      = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port        = htons(SERVER_PORT);

    if ((bind(listenfd, (SA *) &servAddr, sizeof(servAddr))) < 0)
        err_n_die("bind error.");
    
    if ((listen(listenfd, 10)) < 0)
        err_n_die("listen error.");

    
    char *hexToPrint;
    for ( ; ; ) {
        struct sockaddr_in addr;
        socklen_t addr_len;

        printf("waiting for a connection on port %d\n", SERVER_PORT);
        fflush(stdout);
        connfd = accept(listenfd, (SA *) NULL, NULL);

        memset(recoveryLine, 0, MAXLINE);
        memset(tempBuff, 0, MAXLINE);
        memset(endpoint, 0, MAXLINE - 1);


        while ((n = read(connfd, recoveryLine, MAXLINE - 1)) > 0) {
            int i;
            for (i = 5; recoveryLine[i] != ' '; i++) {
                endpoint[i - 5] = recoveryLine[i];
            }
            endpoint[i - 5] = '\0';
            hexToPrint = bin2hex(recoveryLine, n);
            fprintf(stdout, "\n%s\n\n%s", hexToPrint, recoveryLine);
            free(hexToPrint);

            if (recoveryLine[n - 1] == '\n') {
                break;
            }
            memset(recoveryLine, 0, MAXLINE);
        }

        if (n < 0)
            err_n_die("read error.");

        NodePtr tempRoute = search(routes, endpoint);
        if (tempRoute == NULL) {
            strcat(tempBuff, "HTTP/1.1 404 NOT FOUND\r\n\r\n");
            strcat(tempBuff, "<h1>PÁGINA NAO ENCONTRADA!</h1>");
        } else {
            strcat(tempBuff, "HTTP/1.1 200 OK\r\n\r\n");
            strcat(tempBuff, tempRoute->templateName);
        }
        
        snprintf((char *)buff, sizeof(buff), "%s", tempBuff);

        write(connfd, (char *)buff, strlen((char *)buff));
        close(connfd);
    }
}
