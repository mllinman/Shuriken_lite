IF NOT EXISTS (SELECT * FROM sys.tables WHERE name = 'migrations')
BEGIN
    CREATE TABLE migrations (
        id INT IDENTITY(1,1) PRIMARY KEY,
        name NVARCHAR(255) NOT NULL,
        appliedAt DATETIME DEFAULT GETDATE()
    );
END
-- ============================================
-- End of Shuriken Hub Migrations