CREATE TABLE IF NOT EXISTS notifications (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userEmail TEXT NOT NULL,
    message TEXT NOT NULL,
    readFlag INTEGER DEFAULT 0,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(userEmail) REFERENCES users(email)
);
const { pushNotification } = require("./server.js"); // import utility

// inside star route
db.get(`SELECT ownerEmail, name FROM projects WHERE id=?`, [projectId], (err, project) => {
  if (project) {
    pushNotification(project.ownerEmail, `${email} ⭐ starred your project "${project.name}"`);
  }
});
// inside unstar route
db.get(`SELECT ownerEmail, name FROM projects WHERE id=?`, [projectId], (err, project) => {
  if (project) {
    pushNotification(project.ownerEmail, `${email} ❌ unstarred your project "${project.name}"`);
  }
});