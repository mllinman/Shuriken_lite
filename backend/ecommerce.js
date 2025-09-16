const express = require('express');
const stripe = require('stripe')(process.env.STRIPE_SECRET_KEY || 'sk_test_...');
const { db } = require('./server.js');
const router = express.Router();

// Get all products
router.get('/products', (req, res) => {
  const { category, search } = req.query;
  let sql = 'SELECT * FROM products WHERE is_active = 1';
  const params = [];

  if (category) {
    sql += ' AND category = ?';
    params.push(category);
  }

  if (search) {
    sql += ' AND (name LIKE ? OR description LIKE ?)';
    params.push(`%${search}%`, `%${search}%`);
  }

  sql += ' ORDER BY created_at DESC';

  db.all(sql, params, (err, rows) => {
    if (err) {
      return res.status(500).json({ error: err.message });
    }
    res.json(rows);
  });
});

// Get single product
router.get('/products/:id', (req, res) => {
  db.get('SELECT * FROM products WHERE id = ? AND is_active = 1', [req.params.id], (err, row) => {
    if (err) {
      return res.status(500).json({ error: err.message });
    }
    if (!row) {
      return res.status(404).json({ error: 'Product not found' });
    }
    res.json(row);
  });
});

// Get shopping cart
router.get('/cart/:userEmail', (req, res) => {
  const sql = `
    SELECT c.*, p.name, p.description, p.price, p.image_url, 
           (c.quantity * p.price) as total_price
    FROM shopping_cart c 
    JOIN products p ON c.product_id = p.id 
    WHERE c.user_email = ?
  `;

  db.all(sql, [req.params.userEmail], (err, rows) => {
    if (err) {
      return res.status(500).json({ error: err.message });
    }
    res.json(rows);
  });
});

// Add to cart
router.post('/cart', (req, res) => {
  const { userEmail, productId, quantity = 1 } = req.body;

  // Check if item already in cart
  db.get('SELECT * FROM shopping_cart WHERE user_email = ? AND product_id = ?', 
    [userEmail, productId], (err, row) => {
    if (err) {
      return res.status(500).json({ error: err.message });
    }

    if (row) {
      // Update quantity
      db.run('UPDATE shopping_cart SET quantity = quantity + ?, updated_at = CURRENT_TIMESTAMP WHERE user_email = ? AND product_id = ?',
        [quantity, userEmail, productId], function(err) {
        if (err) {
          return res.status(500).json({ error: err.message });
        }
        res.json({ success: true, message: 'Cart updated' });
      });
    } else {
      // Add new item
      db.run('INSERT INTO shopping_cart (user_email, product_id, quantity) VALUES (?, ?, ?)',
        [userEmail, productId, quantity], function(err) {
        if (err) {
          return res.status(500).json({ error: err.message });
        }
        res.json({ success: true, message: 'Item added to cart' });
      });
    }
  });
});

// Update cart item
router.put('/cart/:id', (req, res) => {
  const { quantity } = req.body;
  
  db.run('UPDATE shopping_cart SET quantity = ?, updated_at = CURRENT_TIMESTAMP WHERE id = ?',
    [quantity, req.params.id], function(err) {
    if (err) {
      return res.status(500).json({ error: err.message });
    }
    res.json({ success: true, message: 'Cart updated' });
  });
});

// Remove from cart
router.delete('/cart/:id', (req, res) => {
  db.run('DELETE FROM shopping_cart WHERE id = ?', [req.params.id], function(err) {
    if (err) {
      return res.status(500).json({ error: err.message });
    }
    res.json({ success: true, message: 'Item removed from cart' });
  });
});

// Create payment intent
router.post('/create-payment-intent', async (req, res) => {
  try {
    const { amount, currency = 'usd', userEmail } = req.body;

    const paymentIntent = await stripe.paymentIntents.create({
      amount: Math.round(amount * 100), // Convert to cents
      currency,
      metadata: {
        userEmail: userEmail
      }
    });

    res.json({
      clientSecret: paymentIntent.client_secret,
      paymentIntentId: paymentIntent.id
    });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});

// Create order
router.post('/orders', (req, res) => {
  const { userEmail, items, totalAmount, stripePaymentIntentId, shippingAddress, billingAddress } = req.body;

  db.serialize(() => {
    db.run('BEGIN TRANSACTION');

    // Create order
    db.run('INSERT INTO orders (user_email, total_amount, stripe_payment_intent_id, shipping_address, billing_address) VALUES (?, ?, ?, ?, ?)',
      [userEmail, totalAmount, stripePaymentIntentId, JSON.stringify(shippingAddress), JSON.stringify(billingAddress)], function(err) {
      if (err) {
        db.run('ROLLBACK');
        return res.status(500).json({ error: err.message });
      }

      const orderId = this.lastID;

      // Add order items
      const stmt = db.prepare('INSERT INTO order_items (order_id, product_id, quantity, unit_price, total_price) VALUES (?, ?, ?, ?, ?)');
      
      items.forEach(item => {
        stmt.run([orderId, item.productId, item.quantity, item.unitPrice, item.totalPrice]);
      });

      stmt.finalize((err) => {
        if (err) {
          db.run('ROLLBACK');
          return res.status(500).json({ error: err.message });
        }

        // Clear cart
        db.run('DELETE FROM shopping_cart WHERE user_email = ?', [userEmail], (err) => {
          if (err) {
            db.run('ROLLBACK');
            return res.status(500).json({ error: err.message });
          }

          db.run('COMMIT');
          res.json({ success: true, orderId: orderId });
        });
      });
    });
  });
});

// Get user orders
router.get('/orders/:userEmail', (req, res) => {
  const sql = `
    SELECT o.*, 
           GROUP_CONCAT(p.name || ' x' || oi.quantity) as items
    FROM orders o 
    JOIN order_items oi ON o.id = oi.order_id
    JOIN products p ON oi.product_id = p.id
    WHERE o.user_email = ?
    GROUP BY o.id
    ORDER BY o.created_at DESC
  `;

  db.all(sql, [req.params.userEmail], (err, rows) => {
    if (err) {
      return res.status(500).json({ error: err.message });
    }
    res.json(rows);
  });
});

// Stripe webhook
router.post('/webhook', express.raw({type: 'application/json'}), (req, res) => {
  const sig = req.headers['stripe-signature'];
  let event;

  try {
    event = stripe.webhooks.constructEvent(req.body, sig, process.env.STRIPE_WEBHOOK_SECRET);
  } catch (err) {
    console.log(`Webhook signature verification failed.`, err.message);
    return res.status(400).send(`Webhook Error: ${err.message}`);
  }

  // Handle the event
  switch (event.type) {
    case 'payment_intent.succeeded':
      const paymentIntent = event.data.object;
      // Update order status
      db.run('UPDATE orders SET status = ? WHERE stripe_payment_intent_id = ?',
        ['processing', paymentIntent.id]);
      break;
    
    case 'payment_intent.payment_failed':
      const failedPayment = event.data.object;
      // Update order status
      db.run('UPDATE orders SET status = ? WHERE stripe_payment_intent_id = ?',
        ['cancelled', failedPayment.id]);
      break;

    default:
      console.log(`Unhandled event type ${event.type}`);
  }

  res.json({received: true});
});

module.exports = router;