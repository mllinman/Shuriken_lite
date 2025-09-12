CREATE TABLE IF NOT EXISTS organizations (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    ownerEmail TEXT NOT NULL,
    createdAt TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(ownerEmail) REFERENCES users(email)
);

CREATE TABLE IF NOT EXISTS org_members (
    orgId INTEGER NOT NULL,
    memberEmail TEXT NOT NULL,
    role TEXT CHECK(role IN ('owner','admin','member')) DEFAULT 'member',
    FOREIGN KEY(orgId) REFERENCES organizations(id),
    FOREIGN KEY(memberEmail) REFERENCES users(email),
    UNIQUE(orgId, memberEmail)
);

CREATE TABLE IF NOT EXISTS org_projects (
    orgId INTEGER NOT NULL,
    projectId INTEGER NOT NULL,
    FOREIGN KEY(orgId) REFERENCES organizations(id),
    FOREIGN KEY(projectId) REFERENCES projects(id),
    UNIQUE(orgId, projectId)
);
