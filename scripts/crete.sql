CREATE TABLE users (
	id INT UNSIGNED auto_increment NOT NULL,
	email varchar(254) NOT NULL,
	password varchar(254) NOT NULL,
	jsonData json NULL,
	status INT NOT NULL COMMENT '0 = unactive, 1 = active, 2 = suspended',
	lastLogin DATETIME NULL,
	expiration DATETIME NULL,
	PRIMARY KEY (id)
)
ENGINE=InnoDB
DEFAULT CHARSET=utf8mb4
COLLATE=utf8mb4_general_ci;

CREATE TABLE domain (
	id INT UNSIGNED auto_increment NOT NULL,
	domain varchar(254) NOT NULL,
	user varchar(254) NOT NULL,
	password varchar(254) NOT NULL,
	seecret varchar(254) NOT NULL,
	status INT NOT NULL COMMENT '0 = unactive, 1 = active, 2 = suspended',
	expiration DATETIME NULL,
	PRIMARY KEY (id)
)
ENGINE=InnoDB
DEFAULT CHARSET=utf8mb4
COLLATE=utf8mb4_general_ci;
