-- -----------------------------------------------------
-- Schema web_auth
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `web_auth` ;

-- -----------------------------------------------------
-- Schema web_auth
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `web_auth` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_bin ;
USE `web_auth` ;

-- -----------------------------------------------------
-- Table `web_auth`.`domains`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `web_auth`.`domains` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(254) NOT NULL,
  `user` VARCHAR(254) NOT NULL COMMENT 'Connection user auth',
  `password` VARCHAR(254) NOT NULL COMMENT 'Connection user auth',
  `secret` VARCHAR(254) NOT NULL COMMENT 'JWT secret key',
  `status` TINYINT(1) UNSIGNED NOT NULL DEFAULT 1 COMMENT '0 = unactive\n1 = active\n2 = lock\n',
  `expiration` DATETIME NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `id_UNIQUE` (`id` ASC),
  UNIQUE INDEX `name_UNIQUE` (`name` ASC))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `web_auth`.`users`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `web_auth`.`users` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `id_domain` INT UNSIGNED NOT NULL,
  `email` VARCHAR(254) NOT NULL,
  `password` VARCHAR(254) NOT NULL,
  `json_data` VARCHAR(254) NULL,
  `status` TINYINT(1) UNSIGNED NOT NULL DEFAULT 1 COMMENT '0 = unactive\n1 = active\n2 = lock\n',
  `last_login` DATETIME NULL,
  `expiration` DATETIME NULL,
  PRIMARY KEY (`id`, `id_domain`),
  UNIQUE INDEX `id_UNIQUE` (`id` ASC),
  INDEX `fk_users_domains_idx` (`id_domain` ASC),
  UNIQUE INDEX `unique_email_domain` (`id_domain` ASC, `email` ASC),
  CONSTRAINT `fk_users_domains`
    FOREIGN KEY (`id_domain`)
    REFERENCES `web_auth`.`domains` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

