-- MySQL dump 10.13  Distrib 5.6.22, for Win64 (x86_64)
--
-- Host: localhost    Database: devices
-- ------------------------------------------------------
-- Server version	5.6.22

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `devices`
--

DROP TABLE IF EXISTS `devices`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `devices` (
  `iddevices` int(11) NOT NULL,
  `devices` varchar(128) NOT NULL,
  `idmanuf` varchar(45) DEFAULT NULL,
  `isobsolete` varchar(45) DEFAULT NULL,
  `description` varchar(64) DEFAULT NULL,
  `notes` varchar(45) DEFAULT NULL,
  `url` varchar(255) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  `date` datetime DEFAULT NULL,
  `decimal` varchar(128) DEFAULT NULL,
  `first` varchar(128) DEFAULT NULL,
  `creator` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`iddevices`),
  UNIQUE KEY `iddev_UNIQUE` (`iddevices`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `devices`
--

LOCK TABLES `devices` WRITE;
/*!40000 ALTER TABLE `devices` DISABLE KEYS */;
INSERT INTO `devices` VALUES (1,'Плата вспомогательная объединительная АВ-ТУК-ВС-ОЦ в сборе','2','0','Плата АВ-ТУК-ВС-ОЦ АВМР.426419.009','','//fserver/AV-TUK/Схемы/АВ-ТУК/ВС-ОЦ','2001',0,'2015-11-08 22:26:01','АВМР.426419.009','АВ-ТУК','2001');
/*!40000 ALTER TABLE `devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `documents`
--

DROP TABLE IF EXISTS `documents`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `documents` (
  `iddocuments` int(11) NOT NULL,
  `documents` varchar(128) DEFAULT NULL,
  `device` varchar(128) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` varchar(1) NOT NULL DEFAULT '0',
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddocuments`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `documents`
--

LOCK TABLES `documents` WRITE;
/*!40000 ALTER TABLE `documents` DISABLE KEYS */;
/*!40000 ALTER TABLE `documents` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `lists`
--

DROP TABLE IF EXISTS `lists`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `lists` (
  `idlists` int(11) NOT NULL,
  `iddevices` varchar(128) DEFAULT NULL,
  `idpers` varchar(128) DEFAULT NULL,
  `date` varchar(128) DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idlists`),
  UNIQUE KEY `idlists_UNIQUE` (`idlists`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `lists`
--

LOCK TABLES `lists` WRITE;
/*!40000 ALTER TABLE `lists` DISABLE KEYS */;
/*!40000 ALTER TABLE `lists` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sp`
--

DROP TABLE IF EXISTS `sp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sp` (
  `idsp` int(11) NOT NULL AUTO_INCREMENT,
  `sp` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idsp`),
  UNIQUE KEY `idsp_UNIQUE` (`idsp`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sp`
--

LOCK TABLES `sp` WRITE;
/*!40000 ALTER TABLE `sp` DISABLE KEYS */;
/*!40000 ALTER TABLE `sp` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `spec`
--

DROP TABLE IF EXISTS `spec`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `spec` (
  `idspec` int(11) NOT NULL AUTO_INCREMENT,
  `idsp` varchar(45) DEFAULT NULL,
  `idnk` varchar(45) DEFAULT NULL,
  `quantity` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  PRIMARY KEY (`idspec`),
  UNIQUE KEY `idspec_UNIQUE` (`idspec`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `spec`
--

LOCK TABLES `spec` WRITE;
/*!40000 ALTER TABLE `spec` DISABLE KEYS */;
/*!40000 ALTER TABLE `spec` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-01-10 23:44:09
