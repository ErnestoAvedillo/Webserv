#include <iostream>
#include <stdlib.h>
using namespace std;

const string ENV[24] = {
	"COMSPEC", "DOCUMENT_ROOT", "GATEWAY_INTERFACE",
	"HTTP_ACCEPT", "HTTP_ACCEPT_ENCODING",
	"HTTP_ACCEPT_LANGUAGE", "HTTP_CONNECTION",
	"HTTP_HOST", "HTTP_USER_AGENT", "PATH",
	"QUERY_STRING", "REMOTE_ADDR", "REMOTE_PORT",
	"REQUEST_METHOD", "REQUEST_URI", "SCRIPT_FILENAME",
	"SCRIPT_NAME", "SERVER_ADDR", "SERVER_ADMIN",
	"SERVER_NAME", "SERVER_PORT", "SERVER_PROTOCOL",
	"SERVER_SIGNATURE", "SERVER_SOFTWARE"};

int main(int av, char **ac)
{
	(void)av;
	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title>CGI Environment Variables</title>\n";
	cout << "</head>\n";
	cout << "<body>\n";
	cout << "<table border = \"0\" cellspacing = \"2\">\n";

	cout << "<tr><td>" << "Argumentos de entrada." << "</td></tr>\n";

	for (int i = 1; i < av; i++)
	{
		cout << "<tr><td>" << "Argumento de entrada N." << i << "= " << ac[i] << "</td></tr>\n";
	}

	cout << "</table><\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return 0;
}