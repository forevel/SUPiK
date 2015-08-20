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
  `iddev` int(11) NOT NULL AUTO_INCREMENT,
  `idsp` varchar(45) DEFAULT NULL,
  `idmanuf` varchar(45) DEFAULT NULL,
  `pn` varchar(45) NOT NULL,
  `isobsolete` varchar(45) DEFAULT NULL,
  `description` varchar(64) DEFAULT NULL,
  `notes` varchar(45) DEFAULT NULL,
  `url` varchar(255) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  `date` datetime DEFAULT NULL,
  PRIMARY KEY (`iddev`),
  UNIQUE KEY `iddev_UNIQUE` (`iddev`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `devices`
--

LOCK TABLES `devices` WRITE;
/*!40000 ALTER TABLE `devices` DISABLE KEYS */;
/*!40000 ALTER TABLE `devices` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `modules`
--

DROP TABLE IF EXISTS `modules`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `modules` (
  `idmodules` int(11) NOT NULL,
  `PartNumber` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idmodules`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `modules`
--

LOCK TABLES `modules` WRITE;
/*!40000 ALTER TABLE `modules` DISABLE KEYS */;
/*!40000 ALTER TABLE `modules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pcbs`
--

DROP TABLE IF EXISTS `pcbs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pcbs` (
  `idpcbs` int(11) NOT NULL,
  `PartNumber` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idpcbs`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pcbs`
--

LOCK TABLES `pcbs` WRITE;
/*!40000 ALTER TABLE `pcbs` DISABLE KEYS */;
/*!40000 ALTER TABLE `pcbs` ENABLE KEYS */;
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

--
-- Table structure for table `specification`
--

DROP TABLE IF EXISTS `specification`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `specification` (
  `idspecification` int(11) NOT NULL,
  `specification` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idspecification`),
  UNIQUE KEY `idsp_UNIQUE` (`idspecification`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `specification`
--

LOCK TABLES `specification` WRITE;
/*!40000 ALTER TABLE `specification` DISABLE KEYS */;
/*!40000 ALTER TABLE `specification` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-08-20 22:04:53
