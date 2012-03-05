 Contribuciones al proyecto coconet
====================================

Actualmente hay seis vías principales de contribuir al proyecto. La
primera de ellas es probar el programa e informar de todos aquellos
errores que se hayan descubierto. Si has descubierto algún fallo en el
programa y quieres informar de ello, por favor, visita la sección
Testers de este documento.

La siguiente manera de colaborar con el proyecto es donar algo de
dinero para ayudar a mantener el proyecto. Éste dinero se puede
asignar a mantenimiento del servidor del proyecto (renovar hardware,
renovar dominios, ...) o al pago a un desarrollador para que pueda
trabajar en exclusiva en el proyecto. Para saber cómo donar dinero
al proyecto, por favor, vaya a la sección Donaciones de este
documento.

Si prefieres traducir el programa, la documentación o la página web
del proyecto a cualquier lenguaje en el que no esté disponible, visite
la sección de Traductores en este documento.

Si lo que prefiere es generar o modificar la documentación existente
sobre el proyecto, por favor, visite la sección de Documentalistas en
este documento.

Si quieres contribuir en la administración del servidor del proyecto
(donde se alojan la página web, el repositorio git y todos los
recursos del proyecto), por favor, visite la sección de
Administradores en este documento.

Y, finalmente, si lo que quiere es ayudar desarrollando código para el
proyecto, por favor, visite la sección de Desarrolladores en este
documento.

Si encuentra alguna otra forma de colaborar con el proyecto, por
favor, contacte con palvarez@ritho.net exponiendo su idea.

 Testers
=========

La parte de tests es una de las partes mas importantes de cualquier
proyecto, ya que es la encargada de que el programa funcione tal y
como se espera de el. Si deseas ayudar al proyecto haciendo tests 
puedes continuar por tres vias:

* Probando el programa de todas las maneras que se te ocurran e ir
  reportando todos los errores que vayas encontrando.
* Crear casos de prueba que se ejecuten de manera automatica para
  probar la distinta funcionalidad del programa. Estos casos de prueba
  pueden ser test unitarios, de caja negra, de caja blanca, validacion
  de funciones, ... Basicamente cualquier cosa que se te ocurra.
* Ejecutar los tests automaticos que esten hechos e ir reportando
  todos los fallos que vayan surgiendo.

Los tests que se ejecutan automaticamente se encuentran dentro del
directorio tests, y para ejecutarlos tienes que ejecutar en cualquiera
de los directorios donde haya un Makefile el comando "make tests",
tras lo cual podras ver la salida generada por los tests y los
posibles errores que se generen tanto en la pantalla como en un
fichero generado en el directorio tests.

Para informar sobre un bug visita la pagina del proyecto en github
(https://github.com/i02sopop/coconet), selecciona la seccion Issues y
selecciona "Create a new issue". Por favor, se tan descriptivo como
puedas, incluye toda la informacion que te sea posible y pon un email
de contacto por si necesitamos ponernos en contacto contigo.

Recuerda que lo importante de los tests es encontrar errores, por lo
que es importante que pruebes desde la funcionalidad habitual hasta
situaciones que en condiciones normales no tendrian que surgir nunca,
y no te sientas mal por descubrir bugs, recuerda que con ello estas 
ayudando a mejorar el programa.

 Donaciones
============

Actualmente no existe definido un mecanismo de donacion al proyecto,
pero si tienes ganas de aportar algo al proyecto, ya sea de manera
economica como en especia (ordenadores, teclados, monitores, ...), por
favor, ponte en contacto con <palvarez@ritho.net> con tu propuesta y
buscaremos posibles soluciones.

 Traductores
=============

TODO

 Documentalistas
=================

TODO

 Administradores
=================

El proyecto necesita administradores que actualicen la web,
administren las listas de correo, ... En el repositorio encontraras un
documento con tareas relacionadas con la administracion ordenadas
tanto de mas facil a mas dificil como de mas a menos urgente. Si
quieres colaborar en la administracion de los servidores para el
proyecto lee dicho documento y, sobretodo, por favor, ponte en
contacto con el resto de administradores (pregunta en
<coconet-developers@lists.ritho.net>) o con <palvarez@ritho.net> para
coordinarte con ellos.

 Desarrolladores
=================

El desarrollo de coconet utiliza como sistema de control de versiones
git, cuyo repositorio puedes encontrar en la web del proyecto en
github en https://github.com/i02sopop/coconet. Hay una lista de
desarrolladores en coconet-developers@lists.ritho.net y una lista
donde se envian todos los commits en coconet-commits@lists.ritho.net.
Puedes suscribirte y acceder a estas listas en https://lists.ritho.net.

Si quieres empezar a colaborar en el proyecto puedes revisar el
fichero TODO que esta en el repositorio, el cual es un fichero org
(si no sabes lo que es informate sobre el org-mode de emacs) con
tareas que hay que hacer junto con su descripcion, una estimacion de
su dificultad, la persona que ha incluido la tarea y a la cual le
puede preguntar mas informacion sobre ella y, en su caso, la persona
que tiene asignada esa tarea. Puedes tambien echar un vistazo a la
seccion issues de la pagina del proyecto en github para ver los bugs
que hay abiertos sobre el proyecto.

TODO: Incluir informacion sobre los directorios que hay dentro del
repositorio.
