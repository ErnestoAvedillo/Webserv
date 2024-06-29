#include <iostream>
#include <stdlib.h>
using namespace std;

const string ENV[24] = {
	"SCRIPT_FILENAME", "QUERY_STRING", "REQUEST_METHOD",
	"CONTENT_TYPE", "CONTENT_LENGTH",
	"GATEWAY_INTERFACE", "SERVER_SOFTWARE",
	"REMOTE_ADDR", "REMOTE_PORT", "SERVER_ADDR",
	"SERVER_PORT", "CONTENT_LENGTH", "CONTENT_TYPE",
	"SERVER_NAME", "HTTP_HOST", "HTTP_USER_AGENT",
	"HTTP_ACCEPT", "HTTP_ACCEPT_LANGUAGE", "HTTP_ACCEPT_ENCODING",
	"HTTP_COOKIE", "HTTP_CONNECTION", "HTTP_UPGRADE_INSECURE_REQUESTS",
	"PATH_INFO"}; 

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
		cout << "<tr><td>" << "Argumento de entrada Nº" << i << "= " << ac[i] << "</td></tr>\n";
	}
	for (int i = 0; i < 24; i++)
	{
		cout << "<tr><td>" << ENV[i] << "</td><td>";

		// attempt to retrieve value of environment variable
		char *value = getenv(ENV[i].c_str());
		if (value != 0)
		{
			cout << value;
		}
		else
		{
			cout << "Environment variable does not exist.";
		}
		cout << "</td></tr>\n";
	}

	cout << "</table><\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return 0;
}