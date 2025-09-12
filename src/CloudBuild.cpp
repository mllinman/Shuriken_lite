#include <QNetworkAccessManager>
#include <QHttpMultiPart>
#include <QFile>
#include <QNetworkReply>

void MainWindow::cloudBuild(QString projectPath) {
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QHttpMultiPart *multi = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QVariant("form-data; name=\"source\"; filename=\"project.zip\""));

    QFile *file = new QFile(projectPath + "/project.zip");
    file->open(QIODevice::ReadOnly);
    filePart.setBodyDevice(file);
    file->setParent(multi);
    multi->append(filePart);

    QNetworkRequest req(QUrl("https://shuriken-cloud.example.com/build"));
    QNetworkReply *reply = manager->post(req, multi);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        QByteArray result = reply->readAll();
        QFile out("build_artifacts.zip");
        out.open(QIODevice::WriteOnly);
        out.write(result);
        out.close();
        ui->logOutput->append("Cloud build finished. Artifacts saved.");
    });
}
