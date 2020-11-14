
use admin;
db.createUser({ 
    user:"web-auth", 
    pwd:"web-auth123",
    roles:[{ role: "readWrite", db: "auth"  }]  
});


db.auth.findAndModify({
    query: { _id: "UNIQUE COUNT DOCUMENT IDENTIFIER" },
    update: {
        $inc: {COUNT: 1 },
    }
    writeConcern: 'majority'
});