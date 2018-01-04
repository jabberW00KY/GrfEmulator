#include "emulator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationDisplayName("GrfEmu");
    a.setApplicationName("GrfEmu");
    a.setApplicationVersion("1.0.2");
    a.setOrganizationName("NPP_Tehnotron");

    Emulator w;
    w.show();

    return a.exec();
}
