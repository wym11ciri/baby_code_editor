#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H


#include <QMainWindow>
#include <QObject>
#include <QQuickItem>
#include <QSharedDataPointer>
#include <QWidget>
#include <QDialog>
#include <QTextEdit>

class QTextEdit;

class DebugDialog:  public QDialog
{
public:
    DebugDialog();

     QTextEdit *debugtext;
};

#endif // DEBUGDIALOG_H
