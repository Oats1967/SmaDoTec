========================================================================
    MICROSOFT FOUNDATION CLASS LIBRARY : DoseServer-Projektübersicht
========================================================================


Der Anwendungs-Assistent hat diese DoseServer-DLL erstellt.
Diese DLL zeigt die prinzipielle Anwendung der Microsoft Foundation Classes
und dient als Ausgangspunkt für die Erstellung Ihrer eigenen DLL.

Die Datei enthält eine Zusammenfassung des Inhalts der Dateien für die DoseServer-DLL.

DoseServer.vcproj
    Dies ist die Hauptprojektdatei für VC++-Projekte, die vom Anwendungs-Assistenten erstellt wird. 
    Sie enthält Informationen über die Version von Visual C++, mit der 
    die Datei generiert wurde, über die Plattformen, Konfigurationen und Projektfeatures,
    die mit dem Anwendungs-Assistenten ausgewählt wurden.

DoseServer.h
    Die ist die Hauptheaderdatei für die DLL. Sie deklariert die
    CDoseServerApp-Klasse.

DoseServer.cpp
    Dies ist die wichtigste DLL-Quelldatei. Sie enthält die Klasse CDoseServerApp.
DoseServer.rc
    Hierbei handelt es sich um eine Auflistung aller Ressourcen von Microsoft Windows, die
    vom Programm verwendet werden. Sie enthält die Symbole, Bitmaps und Cursors, die im
    Unterverzeichnis RES gespeichert sind. Diese Datei lässt sich direkt in Microsoft
    Visual C++ bearbeiten.

res\DoseServer.rc2
    Diese Datei enthält Ressourcen, die nicht mit Microsoft 
    Visual C++ bearbeitet wurden. Fügen Sie alle Ressourcen in diese Datei ein,
    die nicht mit dem Ressourcen-Editor bearbeitet werden können.

DoseServer.def
    Diese Datei enthält Informationen über die DLL, die zum
    Ausführen mit Microsoft Windows erforderlich ist. Sie definiert Parameter,
    z.B. den Namen und die Beschreibung der DLL. Die Datei exportiert
    Funktionen der DLL.

/////////////////////////////////////////////////////////////////////////////
Weitere Standarddateien:

StdAfx.h, StdAfx.cpp
    Mit diesen Dateien werden vorkompilierte Headerdateien (PCH)
    mit der Bezeichnung DoseServer.pch und eine vorkompilierte Typdatei mit der Bezeichnung StdAfx.obj erstellt.

Resource.h
    Dies ist die Standardheaderdatei, die neue Ressourcen-IDs definiert.
    Die Datei wird mit Microsoft Visual C++ gelesen und aktualisiert.

/////////////////////////////////////////////////////////////////////////////
Weitere Hinweise:

Der Anwendungs-Assistent verwendet "TODO:"-Kommentare, um Teile des Quellcodes anzuzeigen, die hinzugefügt oder angepasst werden müssen.

/////////////////////////////////////////////////////////////////////////////
