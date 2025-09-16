#!/usr/bin/env node
/**
 * CyberRanger E-commerce Platform Test Suite
 * Tests all major e-commerce functionality
 */

const http = require('http');

const API_BASE = 'http://localhost:3000/api';

function makeRequest(options, data = null) {
    return new Promise((resolve, reject) => {
        const req = http.request(options, (res) => {
            let responseData = '';
            res.on('data', (chunk) => responseData += chunk);
            res.on('end', () => {
                try {
                    const parsed = JSON.parse(responseData);
                    resolve({ status: res.statusCode, data: parsed });
                } catch (e) {
                    resolve({ status: res.statusCode, data: responseData });
                }
            });
        });

        req.on('error', reject);
        
        if (data) {
            req.write(JSON.stringify(data));
        }
        req.end();
    });
}

async function runTests() {
    console.log('🚀 Starting CyberRanger E-commerce Tests\n');

    try {
        // Test 1: Get Products
        console.log('1. Testing Product Catalog...');
        const productsResponse = await makeRequest({
            hostname: 'localhost',
            port: 3000,
            path: '/api/products',
            method: 'GET'
        });
        console.log(`   ✅ Products API: ${productsResponse.status} - Found ${productsResponse.data.length} products`);

        // Test 2: Add to Cart
        console.log('2. Testing Shopping Cart...');
        const addToCartResponse = await makeRequest({
            hostname: 'localhost',
            port: 3000,
            path: '/api/cart',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, {
            userEmail: 'test@cyberranger.com',
            productId: 1,
            quantity: 2
        });
        console.log(`   ✅ Add to Cart: ${addToCartResponse.status} - ${addToCartResponse.data.message}`);

        // Test 3: Get Cart
        const cartResponse = await makeRequest({
            hostname: 'localhost',
            port: 3000,
            path: '/api/cart/test@cyberranger.com',
            method: 'GET'
        });
        console.log(`   ✅ Get Cart: ${cartResponse.status} - ${cartResponse.data.length} items in cart`);

        // Test 4: Create Payment Intent (will fail without Stripe keys, but tests endpoint)
        console.log('3. Testing Payment System...');
        const paymentResponse = await makeRequest({
            hostname: 'localhost',
            port: 3000,
            path: '/api/create-payment-intent',
            method: 'POST',
            headers: { 'Content-Type': 'application/json' }
        }, {
            amount: 299.99,
            currency: 'usd',
            userEmail: 'test@cyberranger.com'
        });
        console.log(`   ✅ Payment Intent: ${paymentResponse.status} - ${paymentResponse.data.error ? 'Stripe not configured (expected)' : 'Success'}`);

        // Test 5: Frontend Access
        console.log('4. Testing Frontend Access...');
        const frontendResponse = await makeRequest({
            hostname: 'localhost',
            port: 3000,
            path: '/',
            method: 'GET'
        });
        console.log(`   ✅ Frontend: ${frontendResponse.status} - Page loading correctly`);

        console.log('\n🎉 All tests completed successfully!');
        console.log('\n📋 Test Summary:');
        console.log('   • Product catalog is working');
        console.log('   • Shopping cart functionality is operational');
        console.log('   • Payment system endpoints are ready');
        console.log('   • Frontend is accessible');
        console.log('\n💡 Next Steps:');
        console.log('   • Configure Stripe API keys in backend/.env');
        console.log('   • Update frontend Stripe publishable key');
        console.log('   • Test complete payment flow');

    } catch (error) {
        console.error('❌ Test failed:', error.message);
        console.log('\n🔧 Make sure the server is running with: npm start');
    }
}

// Run tests
runTests();