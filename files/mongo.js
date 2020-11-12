
db.createUser({ 
    user:"web-auth", 
    pwd:"web-auth123",
    roles:[{ role: "readWrite", db: "web-auth"  }]  
});