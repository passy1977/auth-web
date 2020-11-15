
use admin;
db.createUser({ 
    user:"auth-web", 
    pwd:"auth-web123",
    roles:[{ role: "readWrite", db: "auth-web"  }]  
});


db.auth.createIndex( { "email": 1, "domain": 1 }, { unique: true } )