#include "debugdialog.h"
#include <QtWidgets>

DebugDialog::DebugDialog()
{
    setWindowTitle(tr("DebugInfo"));
    debugtext = new QTextEdit;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(debugtext);
    this->setLayout(mainLayout);
}
