==========================================
Instrucciones de instalación Enable Viacam
==========================================

Este documento describe cómo compilar e instalar Enable Viacam 
para GNU/Linux y Windows.

Linux
=====

- Pre-requisitos
  --------------
  wxWidgets >= 2.6
  OpenCV >= 1.0
  gtk-2.0
  libxtst
  libxext

  Notas Ubuntu: con el fin de compilar en Ubuntu (karmic) y en GGv6 se
  necesitan los siguientes paquetes: libxext-dev, libxtst-dev, libgtk2.0-dev, 
  libwxbase2.8-dev, libwxgtk2.8-dev, libhighgui-dev, libswscale-dev
  
- Compilación e instalación genérica
  ----------------------------------

  ./configure
  make
  make install

- Compilación e instalación genérica (modo de depuración)
  -------------------------------------------------------
  
  ./configure --enable-debug
  make
  make install

- Generación de paquete fuente
  ----------------------------

  ./configure
  make dist

- Generación paquete binario DEB
  ------------------------------

  ./configure
  make deb

- Generación paquetes fuentes DEB
  -------------------------------
  
  ./configure
  make deb-src

  Forma antigua:

	1.- Asegúrese de que config.status no existe (user make distclean)
    2.- Ejecutar: fakeroot debian/rules binary
    3.- Limpieza: fakeroot debian/rules clean

- Generación de paquete RPM
  -------------------------

  ./configure
  make rpm

Windows
=======

- Pre-requisitos
  --------------
  
  wxWidgets = 2.8.x
  OpenCV> = 1.0
  Platform SDK (probado con PSDK-x86_WindowsServer2003_SP1 (2005))
  DirectX SDK 9.0 Update (febrero de 2005) o posterior
  
- Construcción con Visual Studio 2003
  -----------------------------------

  Compilar proyecto
  \Microsoft Platform SDK\Samples\Multimedia\DirectShow\BaseClasses\
  Funciona con VS2003, pero también debería funcionar con versiones más 
  recientes de VS.
  La solución de Visual Studio (.sln) está en el directorio win32/
  Debe establecer las variables de entorno siguientes:

    WXWIN para que apunte a la carpeta wxWidgets
    CVPATH para que apunte a la carpeta opencv1
    PSDK_DIR para que apunte a la carpeta del Platform SDK

  Construir como Release.

  Una vez compilado, es posible que desee crear el paquete instalador.
  Emplee para ello Inno Setup y el fichero .iss bajo win32/