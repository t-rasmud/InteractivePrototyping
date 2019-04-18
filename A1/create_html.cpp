/*
 * Creates an html file
 * that displays the count of
 * the word "Rainbow" in the Serial monitor.
 * Code adapted from https://blog.sourcerer.io/building-a-website-with-c-db942c801aee
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <unistd.h>

using namespace std;
void set_page_title(string title) {
}
void h1_text(string text) {
}
int main() {
  // Output HTML boilerplate
  string html_file;
  while (true) {
    html_file = ""; 
    ifstream infile;
    infile.open("rnb_cnt.txt");
    string sLine;
    string rnb_cnt;

    if (infile.good())
    {
        getline(infile, sLine);         
        rnb_cnt = sLine.data();
    }
    infile.close();

    html_file += "<!doctype html>\n";
    html_file += "<html lang=\"en\">\n";
    html_file += "<head>\n";
    html_file = html_file + "<title>" + "</title>\n";
    html_file += "</head>\n";
    html_file += "<body>\n";
    html_file += "<h1>\n";
    html_file += "<center>";
    html_file = html_file + "Rainbow Count" + "<br>";
    html_file += rnb_cnt + "\n";
    html_file += "</center>";
    html_file += "</h1>\n";
    html_file += "</body>\n";
    html_file += "</html>";

    ofstream myfile;
    myfile.open ("rnb_out.html");
    myfile << html_file;
    myfile.close();
    sleep(5);
  }
  return 0;
}
