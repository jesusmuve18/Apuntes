window.onload = function() {
    let bloques = document.querySelectorAll("li a");

    for(let i=0; i<bloques.length; i++){

        if(bloques[i].getAttribute("href")==""){
            bloques[i].innerHTML = 
            "<img class=\"icon-left\" src=\"icons/folder_icon.svg\" alt=\"Carpeta\">"
            + bloques[i].innerHTML +
            "<img class=\"icon-right\" src=\"icons/arrow_icon.svg\" alt=\"flecha\"></img>";
        } else {
            bloques[i].innerHTML = 
            "<img class=\"icon-left\" src=\"icons/link_icon.svg\" alt=\"Enlace\">"
            + bloques[i].innerHTML;
        }
    }

    window.history.replaceState({}, document.title, "/");
};

