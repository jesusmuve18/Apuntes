%option noyywrap
 /*----- Sección de Declaraciones ----------------*/
%{
    #include <iostream>
    #include <fstream>
    #include <string>

    using namespace std;

    ifstream fichero;
    string resultado;

    string cadena_entre(string inicio, string final, string cadena ){
        int pos_inicio = cadena.find(inicio);
        if (pos_inicio == string::npos) {
            return "";
        }
        pos_inicio += inicio.length();
        int pos_final = cadena.find(final, pos_inicio);
        if (pos_final == string::npos) {
            return "";
        }
        return cadena.substr(pos_inicio, pos_final - pos_inicio);
    }
%}

/*----- Sección de Definiciones ----------------*/

/*----------------Estructuras simples------------*/
BOLD    \<strong\>[^\<]*\<\/strong\>
BLACK    \<b\>[^\<]*\<\/b\>
ITALIC    \<em\>[^\<]*\<\/em\>
UNDERLINED    \<u\>[^\<]*\<\/u\>
SECTION    \<h1\>[^\<]*\<\/h1\>
SUBSECTION    \<h2\>[^\<]*\<\/h2\>
SUBSUBSECTION    \<h3\>[^\<]*\<\/h3\>
PARAGRAPH    \<p\>[^\<]*\<\/p\>
IMG    \<img.*src=\"[^\"]*\".*\>
LINK    \<a\ href=\"[^\"]*\"\>[^\<]*\<\/a\>
NEWLINE \<br\>

/*----------------Estructuras complejas------------*/
ULIST    \<ul\>(\n|\ |\t)*(\<li\>[^\<]*\<\/li\>(\n|\ |\t)*)*\<\/ul\>
OLIST    \<ol\>(\n|\ |\t)*(\<li\>[^\<]*\<\/li\>(\n|\ |\t)*)*\<\/ol\>
TABLE    \<table\>(\n|\ |\t)*(\<tr\>((\n|\ |\t)*(\<td\>[^\<]*\<\/td>|\<th\>[^\<]*\<\/th>)(\n|\ |\t)*)*\<\/tr\>(\n|\ |\t)*)*<\/table\>


/*----------------Estructuras tipicas del html------------*/
HEAD    \<head\>((.|\n)*?)\<\/head\>
BODY_BEGIN    \<body\>
BODY_END    \<\/body\>
IGNORE  (\<!DOCTYPE\ html\>|\<html\ lang=\"[^\"]*\"\>|\<\/html>)


/*----- Sección de Reglas ----------------*/
%%
{BOLD}  {
    string tmp = yytext;
    /* Procesa el contenido entre <strong> y </strong> */
    resultado += "\\textbf{" +  cadena_entre("<strong>", "</strong>", yytext) + "}";
}

{BLACK}  {
    string tmp = yytext;
    /* Procesa el contenido entre <b> y </b> */
    resultado += "\\textbf{" + cadena_entre("<b>", "</b>", yytext) + "}";
}

{ITALIC}  {
    string tmp = yytext;
    /* Procesa el contenido entre <em> y </em> */
    resultado += "\\textit{" + cadena_entre("<em>", "</em>", yytext) + "}";
}

{UNDERLINED}  {
    string tmp = yytext;
    /* Procesa el contenido entre <u> y </u> */
    resultado += "\\underline{" + cadena_entre("<u>", "</u>", yytext) + "}";
}

{SECTION}  {
    string tmp = yytext;
    /* Procesa el contenido entre <h1> y </h1> */
    resultado += "\\section{" + cadena_entre("<h1>", "</h1>", yytext) + "}";
}

{SUBSECTION}  {
    string tmp = yytext;
    /* Procesa el contenido entre <h2> y </h2> */
    resultado += "\\subsection{" + cadena_entre("<h2>", "</h2>", yytext) + "}";
}

{SUBSUBSECTION}  {
    string tmp = yytext;
    /* Procesa el contenido entre <h3> y </h3> */
    resultado += "\\subsubsection{" + cadena_entre("<h3>", "</h3>", yytext) + "}";
}

{PARAGRAPH}  {
    string tmp = yytext;
    /* Procesa el contenido entre <p> y </p> en este caso*/
    /*  en latex es simplemente el codigo separado por lineas en blanco*/
    resultado += cadena_entre("<p>", "</p>", yytext);
}

{ULIST} {
    string tmp = cadena_entre("<ul>", "</ul>", yytext);
    resultado += "\\begin{itemize}\n";
    size_t pos = 0;
    while ((pos = tmp.find("<li>")) != string::npos) {
        string item = cadena_entre("<li>", "</li>", tmp);
        resultado += "\t\\item " + item + "\n";
        tmp = tmp.substr(pos + 4);  // Avanza después del `</li>`
    }
    resultado += "\\end{itemize}\n";
}

{OLIST} {
    string tmp = cadena_entre("<ol>", "</ol>", yytext);
    resultado += "\\begin{enumerate}\n";
    size_t pos = 0;
    while ((pos = tmp.find("<li>")) != string::npos) {
        string item = cadena_entre("<li>", "</li>", tmp);
        resultado += "\t\\item " + item + "\n";
        tmp = tmp.substr(pos + 4);  // Avanza después del `</li>`
    }
    resultado += "\\end{enumerate}\n";
}

{LINK}  {
    resultado += "\\href{" + cadena_entre("<a href=\"", ">", yytext) + "}"
               + "{" + cadena_entre(">", "</a>", yytext) + "}";
}

{NEWLINE} {
    resultado += "\\\\\n";
}

{IMG}  {
    string width = cadena_entre("width=\"", "\"", yytext);
    string height = cadena_entre("height=\"", "\"", yytext);
    string modificadores;


    if(width!="") {
        modificadores += "width=" + width + "px, ";
    }

    if(height!="") {
        modificadores += "height=" + height + "px";
    }

    if(modificadores!=""){
        modificadores = "[" + modificadores + "]";
    }

    resultado += "\\includegraphics" + modificadores + "{" + cadena_entre("src=\"", "\"", yytext) + "}";
}

{TABLE} {
    string tabla_html = yytext;

    // Encuentra la primera fila para determinar el número de columnas
    string primera_fila = cadena_entre("<tr>", "</tr>", tabla_html);
    size_t columnas = 0;
    size_t pos_celda = 0;

    // Cuenta cuántas celdas <td> hay en la primera fila (para la dimensión de la tabla)
    while ((pos_celda = primera_fila.find("<td>", pos_celda)) != string::npos) {
        columnas++;
        pos_celda += 4; // Salta la cadena <td>
    }

    // Es posible que el encabezado se haya hecho con la etiqueta <th>
    if(columnas == 0) {
        pos_celda = 0;

        while ((pos_celda = primera_fila.find("<th>", pos_celda)) != string::npos) {
            columnas++;
            pos_celda += 4; // Salta la cadena <th>
        }
    }

    // Construye el preámbulo de la tabla
    string tabla_latex = "\\begin{tabular}{|";
    
    for(int i=0; i<columnas; i++){
        tabla_latex+= "c|";
    }

    tabla_latex += "}\n\t\\hline\n\t";

    // Procesa cada fila de la tabla
    size_t pos_fila = 0;

    while ((pos_fila = tabla_html.find("<tr>")) != string::npos) {
        string fila = cadena_entre("<tr>", "</tr>", tabla_html);
        size_t pos_celda = 0;
        bool modificado=false;

        // Procesa cada celda de la fila
        while ((pos_celda = fila.find("<td>", pos_celda)) != string::npos) {
            string celda = cadena_entre("<td>", "</td>", fila);
            tabla_latex += celda + " & "; // Agrega contenido de celda seguido de "&"
            fila = fila.substr(pos_celda + 4); // Avanza más allá de `</td>`
            modificado=true;
        }

        if(!modificado) { // Si no se ha modificado es porque no se ha encontrado <td> entonces buscamos <th> (negrita)
            pos_celda = 0;

            // Procesa cada celda de la fila
            while ((pos_celda = fila.find("<th>", pos_celda)) != string::npos) {
                string celda = cadena_entre("<th>", "</th>", fila);
                tabla_latex += "\\textbf{" + celda + "}" + " & "; // Agrega contenido de celda seguido de "&"
                fila = fila.substr(pos_celda + 4); // Avanza más allá de `</th>`
            }
        }

        // Elimina el último "&" y agrega fin de fila
        tabla_latex = tabla_latex.substr(0, tabla_latex.length() - 3); // Quita el último `&`
        tabla_latex += " \\\\\n\t\\hline\n\t";

        tabla_html = tabla_html.substr(pos_fila + 4); // Avanza más allá de `</tr>`
    }

    tabla_latex = tabla_latex.substr(0, tabla_latex.length() - 1); // Quita el último tabulador
    tabla_latex += "\\end{tabular}\n";
    resultado += tabla_latex;
}

{HEAD} {
    string titulo = cadena_entre("<title>", "</title>", yytext);
    string autor = cadena_entre("<meta name=\"author\" content=\"", "\">", yytext);

    if(titulo!="")
        resultado += "\\title{" + titulo + "}\n";

    if(autor!="")
        resultado += "\\author{" + autor + "}\n";   
}

{BODY_BEGIN} {
    resultado += "\\begin{document}\n";
}

{BODY_END} {
    resultado += "\\end{document}\n";
}

{IGNORE} {
    // No se incluye en el resultado
}

.|\n    {
    string tmp = yytext;
    resultado += tmp;
}
%%

/*----- Sección de Procedimientos --------*/
int main(int argc, char *argv[]) {
    if (argc == 2) {
        fichero.open(argv[1]);
        if (!fichero.is_open()) {
            cout << "Error de lectura" << endl;
            return 1;
        }
    } else {
        cout << "Uso: " << argv[0] << " <archivo>" << endl;
        return 1;
    }

    resultado = "\\documentclass[12pt]{article}\n";
    resultado += "\\usepackage{graphicx}\n";
    resultado += "\\usepackage{hyperref}\n";


    // Configura Flex para leer desde el archivo
    yyFlexLexer flujo (&fichero,0);
    flujo.yylex();

    cout << resultado << endl;

    return 0;
}