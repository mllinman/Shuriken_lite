// Following Feed tab
#include <qboxlayout.h>
QWidget *feedTab = new QWidget;
QVBoxLayout *feedLayout = new QVBoxLayout(feedTab);

feedList = new QListWidget;
refreshFeedBtn = new QPushButton("Refresh Feed");

feedLayout->addWidget(feedList);
feedLayout->addWidget(refreshFeedBtn);

tabs->addTab(feedTab, "Following Feed");

// Manual refresh
connect(refreshFeedBtn, &QPushButton::clicked, this, [=]() {
    pm->listFollowingFeed();
});

// Auto refresh every 5 minutes
feedRefreshTimer = new QTimer(this);
connect(feedRefreshTimer, &QTimer::timeout, this, [=]() {
    pm->listFollowingFeed();
});
feedRefreshTimer->start(5 * 60 * 1000); // 5 minutes

// Load feed immediately at startup
pm->listFollowingFeed();

connect(pm, &ProjectManager::followingFeedListed, this, [=](QStringList list) {
    feedList->clear();
    feedList->addItems(list);
});
