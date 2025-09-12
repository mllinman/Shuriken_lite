#include "GuiDesigner.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QGraphicsProxyWidget>

GuiDesigner::GuiDesigner(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Canvas
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene);

    // Property editor (simplified)
    properties = new QTreeWidget;
    properties->setHeaderLabels({"Property", "Value"});

    layout->addWidget(view, 3);
    layout->addWidget(properties, 1);

    setLayout(layout);

    // Example widget
    QPushButton *btn = new QPushButton("Click Me");
    QGraphicsProxyWidget *proxy = scene->addWidget(btn);
    proxy->setFlag(QGraphicsItem::ItemIsMovable);
    proxy->setFlag(QGraphicsItem::ItemIsSelectable);
}
void GuiDesigner::addWidget(QString type) {
    if (type == "QPushButton") {
        QPushButton *btn = new QPushButton("Button");
        QGraphicsProxyWidget *proxy = scene->addWidget(btn);
        proxy->setFlag(QGraphicsItem::ItemIsMovable);
        proxy->setFlag(QGraphicsItem::ItemIsSelectable);
    }
}
    QTreeWidgetItem *item = new QTreeWidgetItem(properties);
    item->setText(0, "text");
    item->setText(1, "Button");
    properties->addTopLevelItem(item);
}
QTreeWidgetItem* GuiDesigner::findPropertyItem(const QString &name) {
    for (int i = 0; i < properties->topLevelItemCount(); ++i) {
        QTreeWidgetItem *item = properties->topLevelItem(i);
        if (item->text(0) == name) return item;
    }
    return nullptr;
}

void GuiDesigner::updateProperty(const QString &name, const QString &value) {
    QTreeWidgetItem *item = findPropertyItem(name);
    if (item) {
        item->setText(1, value);
    }
}

void GuiDesigner::refreshProperties() {
    properties->clear();
    // Repopulate the properties tree (this is just an example)
    updateProperty("text", "Button");
}
void GuiDesigner::exportUi(QString filePath) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << "<ui version=\"4.0\">\n";
        out << "  <class>MainWindow</class>\n";
        out << "  <widget class=\"QMainWindow\" name=\"MainWindow\">\n";
        // Iterate over scene items → dump as XML
        out << "  </widget>\n</ui>\n";
    }
}
void GuiDesigner::previewUi() {
    QWidget *preview = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(preview);

    for (QGraphicsItem *item : scene->items()) {
        if (auto proxy = qgraphicsitem_cast<QGraphicsProxyWidget*>(item)) {
            QWidget *clone = proxy->widget()->metaObject()->newInstance();
            layout->addWidget(clone);
        }
    }

    preview->show();
}
void GuiDesigner::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    // Update property editor based on selected item
    QList<QGraphicsItem*> selected = scene->selectedItems();
    if (selected.size() == 1) {
        // For simplicity, just show "text" property
        properties->clear();
        QTreeWidgetItem *item = new QTreeWidgetItem(properties);
        item->setText(0, "text");
        item->setText(1, "Button");
        properties->addTopLevelItem(item);
    }
}
void CodeEditor::mousePressEvent(QMouseEvent *event) {
    if (event->x() < 30) { // margin area
        QTextCursor cursor = cursorForPosition(event->pos());
        int line = cursor.blockNumber() + 1;
        toggleBreakpoint(line);
        return;
    }
    QPlainTextEdit::mousePressEvent(event);

    // Code completion trigger
    if (!globalCompleter) return;
    if (event->text().isEmpty()) return;

    QTextCursor cursor = textCursor();
    int line = cursor.blockNumber() + 1;
    int col = cursor.positionInBlock() + 1;

    QStringList suggestions = globalCompleter->completeAt(currentFile, line, col);
    if (suggestions.isEmpty()) return;

    QCompleter *comp = new QCompleter(suggestions, this);
    comp->setWidget(this);
    comp->setCompletionMode(QCompleter::PopupCompletion);
    comp->complete(cursorRect());
}
