#include <iostream>
#include <stdlib.h>
using namespace std;

int main(int av, char **ac, char **envp)
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
	cout << "<tr><td>" << "Variables de entorno" << "</td></tr>\n";
	int i = 0;
	while(envp[i] != NULL)
	{
		cout << "<tr><td>" << "N." << i << "= " << envp[i] << "</td></tr>\n";
		i++;
	}
	cout << "</table><\n";
	cout << "</body>\n";
	cout << "</html>\n";

	return 0;
}