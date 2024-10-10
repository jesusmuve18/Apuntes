window.onload = function() {
    let bloques = document.querySelectorAll("li a");

    for(let i=0; i<bloques.length; i++){

        if(bloques[i].getAttribute("href")==""){

          var icon = bloques[i].getAttribute('image');

          if(!icon) {
            icon="icons/folder_icon.svg";
          }

          bloques[i].innerHTML = 
          "<img class=\"icon-left\" src=\""+icon+"\" alt=\"Carpeta\">"
          + bloques[i].innerHTML +
          "<img class=\"icon-right\" src=\"icons/arrow_icon.svg\" alt=\"flecha\"></img>";
          
            
        } else {

          var icon = bloques[i].getAttribute('image');

          if(!icon) {
            icon="icons/link_icon.svg";
          }
          bloques[i].innerHTML = 
          "<img class=\"icon-left\" src=\""+icon+"\" alt=\"Enlace\">"
          + bloques[i].innerHTML;
          
        }
    }
};

document.addEventListener('DOMContentLoaded', function() {
    // Selecciona todos los elementos <ul> con la clase "icon"
    const iconLists = document.querySelectorAll('ul.icon');

    // Recorre cada uno de los elementos y añade un eventListener
    iconLists.forEach(function(icon) {
      icon.addEventListener('click', function(event) {
        // Evitar que los enlaces internos (<a>) dentro del <ul> activen el evento
        const url = this.getAttribute('href');
        if (url) {
        window.location.href = url;
        }
        
      });
    });
  });

