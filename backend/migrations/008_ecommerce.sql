-- Create e-commerce tables for products, orders, and payments

CREATE TABLE IF NOT EXISTS products (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    description TEXT,
    price REAL NOT NULL,
    image_url TEXT,
    category TEXT,
    stock_quantity INTEGER DEFAULT 0,
    is_active BOOLEAN DEFAULT 1,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS orders (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_email TEXT NOT NULL,
    total_amount REAL NOT NULL,
    status TEXT CHECK(status IN ('pending', 'processing', 'shipped', 'delivered', 'cancelled')) DEFAULT 'pending',
    stripe_payment_intent_id TEXT,
    shipping_address TEXT,
    billing_address TEXT,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_email) REFERENCES users(email)
);

CREATE TABLE IF NOT EXISTS order_items (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    order_id INTEGER NOT NULL,
    product_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL,
    unit_price REAL NOT NULL,
    total_price REAL NOT NULL,
    FOREIGN KEY(order_id) REFERENCES orders(id),
    FOREIGN KEY(product_id) REFERENCES products(id)
);

CREATE TABLE IF NOT EXISTS shopping_cart (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_email TEXT NOT NULL,
    product_id INTEGER NOT NULL,
    quantity INTEGER NOT NULL DEFAULT 1,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    updated_at TEXT DEFAULT CURRENT_TIMESTAMP,
    UNIQUE(user_email, product_id),
    FOREIGN KEY(user_email) REFERENCES users(email),
    FOREIGN KEY(product_id) REFERENCES products(id)
);

CREATE TABLE IF NOT EXISTS payment_methods (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_email TEXT NOT NULL,
    stripe_payment_method_id TEXT NOT NULL,
    type TEXT NOT NULL,
    last_four TEXT,
    brand TEXT,
    is_default BOOLEAN DEFAULT 0,
    created_at TEXT DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY(user_email) REFERENCES users(email)
);

-- Insert some sample products
INSERT OR IGNORE INTO products (name, description, price, image_url, category, stock_quantity) VALUES
('CyberRanger Pro License', 'Professional license for CyberRanger security tools', 299.99, '/images/cyberranger-pro.jpg', 'Software', 100),
('CyberRanger Enterprise', 'Enterprise edition with advanced features', 999.99, '/images/cyberranger-enterprise.jpg', 'Software', 50),
('Security Assessment Tool', 'Comprehensive security assessment and monitoring', 149.99, '/images/security-tool.jpg', 'Tools', 75),
('Vulnerability Scanner', 'Advanced vulnerability detection system', 199.99, '/images/vuln-scanner.jpg', 'Tools', 60),
('Network Monitor Pro', 'Real-time network monitoring solution', 399.99, '/images/network-monitor.jpg', 'Software', 40);