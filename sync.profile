%modules = (
    "QtInstagram" => "$basedir/src/instagram",
);
%mastercontent = (
    "core" => "#include <QtCore/QtCore>",
    "network" => "#include <QtNetwork/QtNetwork>",
    "xml" => "#include <QtXml/QtXml>",
);
%modulepris = (
    "QtInstagram" => "$basedir/modules/qt_instagram.pri",
);

%dependencies = (
        "qtbase" => "",
);
