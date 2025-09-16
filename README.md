# CyberRanger - Modern E-commerce Platform

## Overview
CyberRanger has been transformed from a C++ IDE into a modern e-commerce platform specializing in cybersecurity tools and services. The platform features comprehensive Stripe payment integration, product management, shopping cart functionality, and a responsive web interface.

## Architecture

### Backend (Node.js + Express)
- **Server**: `backend/server.js` - Main application server with Socket.IO support
- **E-commerce API**: `backend/ecommerce.js` - Complete e-commerce REST API
- **Database**: SQLite with comprehensive migration system
- **Payment Processing**: Stripe integration with webhooks support

### Frontend (HTML5 + JavaScript)
- **Main Interface**: `frontend/index.html` - Modern responsive e-commerce interface
- **Features**: Product catalog, shopping cart, secure checkout with Stripe Elements
- **Design**: Modern gradient design with smooth animations and responsive layout

### Database Schema
- **Products**: Product catalog with categories, pricing, and inventory
- **Orders**: Complete order management with status tracking
- **Shopping Cart**: Persistent cart functionality per user
- **Users**: User authentication and profile management
- **Payment Methods**: Stored payment method management

## Features

### E-commerce Core
- ✅ Product catalog with search and filtering
- ✅ Shopping cart with add/remove/update functionality
- ✅ Secure checkout process with Stripe Elements
- ✅ Order management and tracking
- ✅ Inventory management
- ✅ User authentication system

### Payment Integration
- ✅ Stripe payment intent creation
- ✅ Secure card processing with Stripe Elements
- ✅ Webhook handling for payment events
- ✅ Payment method storage
- ✅ Order status automation

### API Endpoints
- `GET /api/products` - Get all products with filtering
- `GET /api/products/:id` - Get single product
- `GET /api/cart/:userEmail` - Get user's cart
- `POST /api/cart` - Add item to cart
- `PUT /api/cart/:id` - Update cart item
- `DELETE /api/cart/:id` - Remove from cart
- `POST /api/create-payment-intent` - Create Stripe payment intent
- `POST /api/orders` - Create new order
- `GET /api/orders/:userEmail` - Get user orders
- `POST /api/webhook` - Stripe webhook handler

## Setup Instructions

### 1. Backend Setup
```bash
cd backend
npm install
```

### 2. Environment Configuration
Copy `.env.example` to `.env` and update with your Stripe keys:
```bash
cp .env.example .env
# Edit .env with your actual Stripe keys
```

### 3. Database Setup
```bash
node migrate.js
```

### 4. Start Server
```bash
node server.js
```

The server will start on `http://localhost:3000`

## Stripe Configuration

### Test Keys
For development, use Stripe test keys:
- **Publishable Key**: `pk_test_...` (frontend)
- **Secret Key**: `sk_test_...` (backend)
- **Webhook Secret**: `whsec_...` (webhook verification)

### Production Setup
1. Create Stripe account at https://stripe.com
2. Get live API keys from Stripe Dashboard
3. Set up webhook endpoint: `https://yourdomain.com/api/webhook`
4. Update environment variables with live keys

## Product Management

### Adding Products
Products can be added directly to the database or via SQL:
```sql
INSERT INTO products (name, description, price, image_url, category, stock_quantity) 
VALUES ('Product Name', 'Description', 99.99, '/images/product.jpg', 'Category', 100);
```

### Sample Products Included
- CyberRanger Pro License ($299.99)
- CyberRanger Enterprise ($999.99)
- Security Assessment Tool ($149.99)
- Vulnerability Scanner ($199.99)
- Network Monitor Pro ($399.99)

## Testing

### Manual Testing
1. Open `http://localhost:3000` in browser
2. Browse products and add to cart
3. Proceed to checkout
4. Use Stripe test card: `4242 4242 4242 4242`

### API Testing
```bash
# Get products
curl http://localhost:3000/api/products

# Add to cart
curl -X POST http://localhost:3000/api/cart \
  -H "Content-Type: application/json" \
  -d '{"userEmail":"test@example.com","productId":1,"quantity":1}'

# Get cart
curl http://localhost:3000/api/cart/test@example.com
```

## Security Features
- CORS enabled for cross-origin requests
- JWT token authentication for API endpoints
- Stripe secure payment processing
- SQL injection prevention with parameterized queries
- Input validation and sanitization

## Future Enhancements
- User registration and authentication UI
- Admin panel for product management
- Advanced inventory tracking
- Email notifications
- Subscription management
- Multi-currency support
- Advanced analytics and reporting

## Support
For issues or questions, refer to the API documentation or check the server logs for debugging information.