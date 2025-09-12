CREATE TABLE organizations (
    id INT IDENTITY(1,1) PRIMARY KEY,
    name NVARCHAR(255) NOT NULL,
    ownerEmail NVARCHAR(255) NOT NULL,
    createdAt DATETIME DEFAULT GETDATE(),
    FOREIGN KEY(ownerEmail) REFERENCES users(email)
);

CREATE TABLE org_members (
    orgId INT NOT NULL,
    memberEmail NVARCHAR(255) NOT NULL,
    role NVARCHAR(20) CHECK(role IN ('owner','admin','member')) DEFAULT 'member',
    FOREIGN KEY(orgId) REFERENCES organizations(id),
    FOREIGN KEY(memberEmail) REFERENCES users(email),
    CONSTRAINT UQ_org_member UNIQUE(orgId, memberEmail)
);

CREATE TABLE org_projects (
    orgId INT NOT NULL,
    projectId INT NOT NULL,
    FOREIGN KEY(orgId) REFERENCES organizations(id),
    FOREIGN KEY(projectId) REFERENCES projects(id),
    CONSTRAINT UQ_org_project UNIQUE(orgId, projectId)
);
