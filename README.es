                                ---------------
                                 Enable Viacam
                                ---------------
                                  
             emulador de ratón multi-plataforma a través de cámara web

 Acerca de
===========

Enable Viacam (alias eViacam) es un programa de ordenador que sustituye la funcionalidad del ratón permitiendo mover el puntero a partir del movimiento de la cabeza. Funciona en un ordenador PC equipado con una cámara web, sin elementos adicionales. eViacam está basado en el programa Ratón Facial galardonado con varios premios.

 Plataformas soportadas
========================

GNU/Linux
Windows 2000/XP/Vista/7

 Instrucciones
==============

(NOTA: a partir de la versión 1.4, el manual de instrucciones se incluye en el software)

Primero hay que haber instalado y configurar correctamente la cámara web. Es muy importante configurar al cámara en modo de exposición manual y ajustar este parámetro para obtener 25 fps o más de velocidad de captura. La mayoría de cámaras modernas proporcionan hasta 30 fps. Consulte el manual de la cámara en caso necesario.

Una vez instalado, ejecute evicam (evicam.exe para Windows). El programa debe abrirse, detectar automáticamente la cámara y mostrar la imagen en tiempo real.

Sitúe la cámara por obtener una imagen frontal de su rostro. La anchura de este, en la imagen, debe ser aproximadamente un tercio de la anchura del cuadro. Hay que centrar el rectángulo (azul) de seguimiento alrededor de su rostro moviendo la cámara o desplazando el cuadro con el ratón (click en la esquina).

Active la opción para iniciar la función de control desde el menú Archivo. Pruebe a mover la cabeza y observe el resultado. Puede ajustar la sensibilidad (velocidad ejes X e Y) a través del diálogo de configuración. Debe poder hacer llegar el puntero a cualquier lugar de la pantalla.

Para hacer clic sólo hace falta parar el puntero sobre la localización desea. En unos instantes se genera un clic de forma automática (este tiempo de espera se puede ajustar desde el diálogo de configuración). Utilice la ventana empotrada de la parte superior para escoger entre los diferentes tipos de clic.

 Instalación
==============

Ver archivo INSTALL

 Notas
=======

- Idioma Asturiano (Bable)

La versión estable actual de wxWidgets (2.8.x en noviembre de 2010) no es compatible con lengua asturiana aún, pero parece que se ha añadido soporte para la versión de desarrollo. Por lo tanto, si desea activar el soporte de lengua asturiana, necesita la última versión de desarrollo de wxWidgets y hay descomentar la siguiente línea antes de compilar

	// # Define ENABLE_ASTURIAN
en:
	src/langnames-utf8.h
	src/langnames-utf16.h

Tenga en cuenta que estos archivos utilizan una codificación especial.

 Licencia
===========

Enable Viacam se proporciona bajo licencia GNU/GPL v3.
Vea el archivo COPYING para detalles sobre la licencia.

 Autor
========
 
(C) 2008-10 Cesar Mauri Loba (cesar a crea-si punto com)
Vea los archivos AUTHORS y THANKS para más información sobre otras contribuciones.

 Agradecimientos
=================

- Este software ha sido desarrollado gracias al apoyo de diferentes
  colaboradores y el patrocinio de:
	- Consorcio Fernando de los Ríos
    - Seceretaría de Telecomunicaciones y Sociedad de la Información del
	Departamento de Gobernación y Administraciones Públicas de la Generalitat
	de Cataluña
	- Asociación Provincial de Parálisis Cerebral (APPC) de Tarragona.

- Gracias a toda la gente de la APPC por su colaboración en las pruebas y especialmente a los usuarios que han colaborado activamente, entre ellos el Ruben, Gloria, Cristina y muchos y muchos otros. Gracias a todos.

 Enlaces
=========

http://viacam.org
	- Enable Viacam website
http://www.wxwidgets.org
        - wxWidgets toolkit homepage
http://sourceforge.net/projects/opencvlibrary/
	- Open Computer Vision Library homepage
http://www.innosetup.org

http://www.crea-si.com
	- CREA Software Systems site