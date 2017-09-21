//#include <stdlib.h>
//#include <stdio.h>
//#include <stdint.h>
//#include <arpa/inet.h>
//#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
typedef char* string;
typedef struct sockaddr_in InformacionDeDireccionDeSocket_v2;
typedef struct sockaddr DireccionDeSocket;
typedef unsigned int Natural;
InformacionDeDireccionDeSocket_v2 crearDireccionDeServidor(){
	InformacionDeDireccionDeSocket_v2 direccionDeSocket;
	direccionDeSocket.sin_family = AF_INET;
	direccionDeSocket.sin_port = htons(8080);
	return direccionDeSocket;
}
int  verificarError(int n, string s){
	if(n==-1 || n !=0){
		printf(strcat("error en: ",s));
		return 1;
	}
	else{
		printf("ok en %s \n ",s);
	}
}
void barraN(){
	puts("\n");
}
void bindear(int sockfd,InformacionDeDireccionDeSocket_v2 dir){
	int len = sizeof(DireccionDeSocket);
	int loDeBind = bind(sockfd,(DireccionDeSocket*)&dir,len);
	verificarError(loDeBind,"bind");
	free(&len);
	free(&loDeBind);
}
void configurandoConexion(int sockett , int limiteDeEscuchas){

	printf("Estoy escuchando\n");
	listen(sockett, 100);
}
void recibiendoConexion(int sock){
	InformacionDeDireccionDeSocket_v2 otro;
	int cliente = accept(sock, (DireccionDeSocket*)&otro,sizeof(DireccionDeSocket));
	printf("Recibí una conexión en %d!!\n", cliente);
	send(cliente, "Hola NetCat!", 13, 0);
	send(cliente, ":)\n", 4, 0);
	string buffer = malloc(1000);
	while (1) {
		int bytesRecibidos = recv(cliente, buffer, 1000, 0);
		if (bytesRecibidos <= 0) {
			perror("El chabón se desconectó o bla.");
			return ;
		}

		buffer[bytesRecibidos] = '\0';
		printf("Me llegaron %d bytes con %s\n", bytesRecibidos, buffer);
	}

	free(buffer);


}

int main(void) {
	InformacionDeDireccionDeSocket_v2 direccionServidor = crearDireccionDeServidor();

	int servidor = socket(AF_INET, SOCK_STREAM, 0);

	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
	bindear(servidor,direccionServidor);//bindea == relacionar socket y puerto
	configurandoConexion(servidor,3);

	//------------------------------

	//abajo resumi
	recibiendoConexion(servidor);


	return 0;
}
