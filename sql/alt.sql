-- MySQL dump 10.13  Distrib 5.6.29, for Win32 (AMD64)
--
-- Host: localhost    Database: altium
-- ------------------------------------------------------
-- Server version	5.6.29

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
-- Table structure for table `capasitors`
--

DROP TABLE IF EXISTS `capasitors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `capasitors` (
  `id` int(11) NOT NULL,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=30 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `capasitors`
--

LOCK TABLES `capasitors` WRITE;
/*!40000 ALTER TABLE `capasitors` DISABLE KEYS */;
INSERT INTO `capasitors` VALUES (1,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','//fserver/PCAD/Altium/Libs/Sim/Capasitors/Murata/GRM188R71E104KA01.mod','','','НКП','Чип конд. 0,1 мкФ,25 В,X7R,тип 0603','0603','104','0,1 мкФ','25 В','X7R','-55','125','','',' ','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',10,0,'Чип конд. 0,1 мкФ,25 В,X7R,тип 0603','','2016-02-11 12:25:28',2001,0,'',1,'0,1',38,' ',' '),(5,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип конд. 0,47 мкФ,25 В,X7R,тип 0603','0603','','0,47 мкФ','25 В','X7R','-55','125','','',' ','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',6,0,'Чип Конд. 0,47 мкФ,X7R,тип 0603','','2015-09-20 14:54:06',2001,0,'',1,'',0,' ',' '),(6,'Polar Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','D (7343)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип тант. Конд. 220 мкФ,6,3 В,10%,тип D','D','227','220 мкФ','6,3 В','10%','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Vishay\\293d.pdf',5,0,'Чип тант. Конд. 220 мкФ,10%,тип D','','2014-02-28 14:34:00',2001,0,'Чип тант.',1,'220',6,NULL,NULL),(7,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 0,33 мкФ,X7R,0805','0805','','0,33 мкФ','25 В','X7R','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\DOC000250694.pdf',2,0,'Чип Конд. 0,33 мкФ,X7R,тип 0805','','2014-02-28 14:33:49',2008,0,'Чип',1,'0,33',6,NULL,NULL),(8,'Polar Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','E1010','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','Panasonic','EEEFC1V221','1010','220','220 мкФ','35 В','20%','-40','+105','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Panasonic\\V_E.pdf',4,0,'Электролит. конд. EEEFC1V221','Panasonic','2014-02-28 14:33:40',2001,0,'',1,'220',6,NULL,NULL),(9,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','1206 (3216)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 1 мкФ,50 В,10%,тип 1206','1206','105','1 мкФ','50 В','10%','-55','+125','','1500 ppm/C','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\Yageo MLCC X7R_6.3V-to-50V_9.pdf',2,0,'Чип Конд. 1 мкФ,10%,тип 1206','50 В','2014-02-28 14:33:27',2001,0,'Чип',1,'1',6,NULL,NULL),(10,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 1000 пФ,25 В,10%,тип 0603','0603','102','1000 пФ','25 В','10%','-55','+125','','1200 ppm/C','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\Yageo MLCC X7R_6.3V-to-50V_9.pdf',2,0,'Чип Конд. 1000 пФ,10%,тип 0603','','2014-02-28 14:33:15',2001,0,'Чип',1,'1000',5,NULL,NULL),(11,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 2200 пФ,X7R,0603','0603','','2200 пФ','25 В','X7R','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',2,0,'Чип Конд. 2200 пФ,X7R,тип 0603','','2014-02-28 14:33:06',2008,0,'Чип',1,'2200',5,NULL,NULL),(12,'Polar Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','B (3528)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип тант. Конд. 22 мкФ,10 В,10%,тип B','B','226','22 мкФ','10 В','10%','-55','+85','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Vishay\\293d.pdf',2,0,'Чип тант. Конд. 22 мкФ,10%,тип B','','2014-02-28 14:33:00',2001,0,'Чип тант.',1,'22',6,NULL,NULL),(13,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 10 пФ,NPO,0603','0603','','10 пФ','50 В','NPO','','-55','125','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\DOC000250694.pdf',2,0,'Чип Конд. 10 пФ,NPO,тип 0603','','2014-02-28 14:32:41',2008,0,'Чип',1,'10',5,NULL,NULL),(14,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','1206 (3216)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 22 мкФ,X7R,1206','1206','','22 мкФ','25 В','X7R','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\DOC000250694.pdf',3,0,'Чип Конд. 22 мкФ,X7R,тип 1206','','2014-02-28 14:32:30',2008,0,'Чип',1,'22',6,NULL,NULL),(15,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','1206 (3216)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 2,2 мкФ,X7R,тип 1206','1206','','2,2 мкФ','25 В','X7R','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\DOC000250694.pdf',3,0,'Чип Конд. 2,2 мкФ,X7R,тип 1206','','2014-02-28 14:32:19',2001,0,'Чип',1,'2,2',6,NULL,NULL),(16,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 33 пФ,NP0,тип 0603','0603','','33 пФ','50 В','NP0','-55','+125','','30 ppm/C','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\DOC000250694.pdf',3,0,'Чип Конд. 33 пФ,NP0,тип 0603','','2014-02-28 14:32:07',2001,0,'Чип',1,'33',5,NULL,NULL),(17,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип конд. 18 пФ,50 В,NP0,тип 0603','0603','','18 пФ','50 В','NP0','-55','+125','','30 ppm/C',' ','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\DOC000250694.pdf',4,0,'Чип Конд. 18 пФ,NP0,тип 0603','','2016-02-11 13:33:39',2008,0,'',1,'18',38,' ',' '),(18,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 4,7 мкФ,X5R,тип 0805','0805','','4,7 мкФ','10 В','X5R','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',2,0,'Чип Конд. 4,7 мкФ,X5R,тип 0805','GRM21BR61A475KA73L','2014-02-28 14:31:37',2008,0,'Чип',1,'4,7',6,NULL,NULL),(19,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 6,8 пФ,NP0,тип 0805','0805','','6,8 пФ','50 В','NP0','-55','+125','','Yageo CC08','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\CC Series.pdf',4,0,'Чип Конд. 6,8 пФ,NP0,тип 0805','Yageo CC0805CRNPO9BN6R8','2014-02-28 14:32:48',2001,0,'Чип',1,'6,8',5,NULL,NULL),(20,'Polar Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','EECS5R5V','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','Panasonic','EECS5R5V105','EECS5R5V','','1000 мФ','5,5 В','','-25','70','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Panasonic\\Ионисторы Panasonic.pdf',1,0,'Ионистор EECS5R5V105 1Ф 5,5 В','Panasonic','2014-04-02 11:30:36',2008,0,'',0,'1000000',6,NULL,NULL),(21,'Polar Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','E1010','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','Yageo','Чип конд. электролит. 1000 мкФх10 В (1010)','1010','1000','1000 мкФ','10 В','20 %','-40','+105','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\E-Cap_SurfaceMount_CA_2011.pdf',1,0,'Чип конд. электролит. 1000 мкФх10 В (1010)','Yageo','2014-05-23 16:20:02',2001,0,'',1,'1000',6,NULL,NULL),(22,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','1206 (3216)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 0,022 мкФ,X7R,тип 1206','1206','','0,022 мкФ','200 В','X7R','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\CC Series.pdf',2,0,'Чип Конд. 0,022 мкФ,X7R,тип 1206','C1206C223K2R, C3216X7R2E223K','2014-05-26 13:25:37',2008,0,'Чип',1,'0,022',6,NULL,NULL),(23,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','K73','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Плен. Конд. 0,33 мкФ,тип  К73-17',' К73-17','','0,33 мкФ','630 В','10%','-60','+125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\k73_17.pdf',6,0,'Плен. Конд. 0,33 мкФ,тип  К73-17','К73-17-630В-0,33мкФ','2014-06-05 15:11:19',2008,0,'Плен.',0,'0,33',6,NULL,NULL),(24,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','K73-17-620V-022mF-30deg','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Плен. Конд. 0,22 мкФ,тип К73-17','К73-17','','0,22 мкФ','400 В','10%','-60','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\k73_17.pdf',7,0,'Плен. Конд. 0,22 мкФ,тип К73-17','К73-17-400 В-0,22 мкФ','2014-06-09 13:44:21',2008,0,'Плен.',0,'0,22',6,NULL,NULL),(25,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 2,2 мкФ,10 В,тип 0603','0603','','2,2 мкФ','10 В','X7R','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\DOC000250694.pdf',1,0,'Чип Конд. 2,2 мкФ,10 В,тип 0603','GRM188R71A225KE15D','2014-07-22 13:58:58',2008,0,'Чип',1,'2,2',2,NULL,NULL),(26,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','FSERVERPCADAltiumLibsSimCapasitorsMurataGRM188R71E104KA01.mod','','','Amphenol','Чип Конд. 560 пФ,тип 0603','0603','','560 пФ','25V','X7R','-55','125','','','','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',2,0,'Чип Конд. 560 пФ,тип 0603','','2014-07-28 16:46:00',2010,0,'Чип',1,'560',4,NULL,NULL),(27,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','FSERVERPCADAltiumLibsSimCapasitorsMurataGRM188R71E104KA01.mod','','','Amphenol','Чип Конд. 0,022 мкФ,тип 0603','0603','','0.022 мкФ','25V','X7R','-55','125','','','','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',1,0,'Чип Конд. 0,022 мкФ,тип 0603','','2014-07-28 16:47:40',2010,0,'Чип',1,'0.022',2,NULL,NULL),(28,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','JFB02J224K150','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Плен. Конд. 0,22 мкФ,тип ','','2J224K','0,22 мкФ','630 в','10%','-40','+85','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\JB.pdf',1,0,'Плен. Конд. 0,22 мкФ,тип ','JFB02J224K150000B','2014-07-29 18:12:15',2008,0,'Плен.',0,'0,22',2,NULL,NULL),(29,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','JFB02J334K200','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Плен. Конд. 0,33 мкФ,тип 0603','','2J334K','0,33 мкФ','630 В','10%','-40','+85','','','','\\\\fserver\\PCAD\\Altium\\Libs\\DSheet\\Capasitors\\JB.pdf',8,0,'Плен. Конд. 0,33 мкФ,тип ','JFB02J334K200000B','2015-09-18 23:21:47',2001,0,'',0,'',0,'',''),(30,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','FSERVERPCADAltiumLibsSimCapasitorsMurataGRM188R71E104KA01.mod','','','Amphenol','Чип Конд. 100 пФ,тип 0603','0603','','100 пФ','25V','X7R','-55','125','','','','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',1,0,'Чип Конд. 100 пФ,тип 0603','','2014-08-12 14:39:01',2010,0,'Чип',1,'100',4,NULL,NULL),(31,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','FSERVERPCADAltiumLibsSimCapasitorsMurataGRM188R71E104KA01.mod','','','НКП','Чип Конд. 0,01 мкФ,50V,тип 0603','0603','','0,01 мкФ','50V','X7R','-55','125','','','','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',1,0,'Чип Конд. 0,01 мкФ,50V,тип 0603','','2014-08-13 17:42:05',2008,0,'Чип',1,'0,01',2,NULL,NULL),(32,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','FSERVERPCADAltiumLibsSimCapasitorsMurataGRM188R71E104KA01.mod','','','НКП','Чип Конд. 10 мкФ,X5R,тип 0603','0603','','10 мкФ','10V','X5R','-55','125','','','','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GRM188R71E104KA01-01.pdf',1,0,'Чип Конд. 10 мкФ,X5R,тип 0603','','2014-08-14 14:36:31',2008,0,'Чип',1,'10',2,NULL,NULL),(33,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','FSERVERPCADAltiumLibsSimCapasitorsMurataGRM188R71E104KA01.mod','','','Murata','Чип Конд. 47 пФ,тип 0603','0603','','47 пФ','50V','X8G','-55','125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Murata\\GCM1885G1H4470GA16.pdf',1,0,'Чип Конд. 47 пФ,тип 0603','','2014-08-15 11:00:40',2010,0,'Чип',1,'47',4,NULL,NULL),(34,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 0,1 мкФ,50 В,X7R,тип 0603','0603','','0,1 мкФ','50 В','X7R','-40','+125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\Yageo MLCC X7R_6.3V-to-50V_9.pdf',2,0,'Чип Конд. 0,1 мкФ,50 В,X7R,тип 0603','CC0603KRX7R9BB104','2014-12-11 13:58:23',2008,0,'Чип',1,'0,1',2,NULL,NULL),(35,'Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип Конд. 0,1 мкФ,тип 0805','0805','','0,1 мкФ','50 В','X7R','-40','+125','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\Yageo MLCC X7R_6.3V-to-50V_9.pdf',2,0,'Чип Конд. 0,1 мкФ,тип 0805','','2015-07-23 17:23:04',2001,0,'Чип',1,'0,1',2,NULL,NULL),(36,'Polar Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','E0405','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип электролит. Конд. 2,2 мкФ,50 В,тип 4х5,4','4х5,4','','2,2 мкФ','50 В','20 %','-40','+85','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\E-Cap_SurfaceMount_CA_2011.pdf',1,0,'Чип электролит. Конд. 2,2 мкФ,50 В,тип 4х5,4','','2015-06-17 17:21:12',2001,0,'Чип электролит.',1,'2,2',2,NULL,NULL),(37,'Polar Capasitor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\capasitors.SchLib','E0405','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\capasitors.PcbLib','','','','','НКП','Чип электролит. Конд. 10 мкФ,16 В,тип 4х5,4','4х5,4','','10 мкФ','16 В','20 %','-40','+85','','','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Capasitors\\Yageo\\E-Cap_SurfaceMount_CA_2011.pdf',1,0,'Чип электролит. Конд. 10 мкФ,16 В,тип 4х5,4','','2015-06-22 14:02:13',2001,0,'Чип электролит.',1,'10',2,NULL,NULL);
/*!40000 ALTER TABLE `capasitors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `connections`
--

DROP TABLE IF EXISTS `connections`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `connections` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `connections`
--

LOCK TABLES `connections` WRITE;
/*!40000 ALTER TABLE `connections` DISABLE KEYS */;
INSERT INTO `connections` VALUES (1,'Hole','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connections.SchLib','Hole 1,5','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connections.PcbLib','','','','','НКП','Отв. 1,5','','','Hole ','','','','','','',2001,'2014-07-22 15:22:56','','',7,0,'Отв. 1,5','',0,'Отв. 1,5',0,'Hole',0,NULL,NULL),(2,'Hole','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connections.SchLib','Hole 0.8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connections.PcbLib','','','','','НКП','Отв. 0,8','','','Hole ','','','','','','',2001,'2014-05-23 12:38:37','','',5,0,'Отв. 0,8','',0,'Отв. 0,8',0,'Hole',0,NULL,NULL),(3,'Hole','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connections.SchLib','HOLE 2.0','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connections.PcbLib','','','','','ООО АСУ-ВЭИ','Отв. 2.0','','',' ','','','','','','',2010,'2014-07-22 15:22:19','','',5,1,' ','',0,'',0,'',0,NULL,NULL),(4,'Hole','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connections.SchLib','HOLE 3.5','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connections.PcbLib','','','','','ООО АСУ-ВЭИ','Отв. 3.5','','',' ','','','','','','',2010,'2014-07-22 14:31:42','','',1,1,' ','',0,'',0,'',0,NULL,NULL),(5,'Hole 2x20','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connections.SchLib','Отв. под WR-PHD40','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connections.PcbLib','','','','','НКП','Отв. под WR-PHD40','','',' ','','','','','','',2001,'2015-05-13 14:04:13','','',1,0,'Отв. под WR-PHD40','',0,'Отв. под WR-PHD40',0,'',0,NULL,NULL);
/*!40000 ALTER TABLE `connections` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `connectors`
--

DROP TABLE IF EXISTS `connectors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `connectors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=77 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `connectors`
--

LOCK TABLES `connectors` WRITE;
/*!40000 ALTER TABLE `connectors` DISABLE KEYS */;
INSERT INTO `connectors` VALUES (1,'XP-3','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','Wago 770-813/011-000','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','WAGO Kontakttechnik GmbH & Co. KG','770-813/011-000','770-813/011-000','','20 А','600 В','','-35','85','','',2001,'2014-02-28 14:36:41','770-813/011-000','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wago\\wago 770-813_011-000.pdf',4,0,'Розетка угловая на плату 770-813/011-000','Wago',0,'',0,18,'20',NULL,NULL),(2,'XP-8','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','TE 5646958-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','5646958-1','5646958-1','','10 А','250 В','','-55','125','','',2001,'2014-02-28 14:36:50','5646958-1','\\\\FSERVER\\Pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_5646958_C.pdf',4,0,'Вилка угловая на плату UPM 5646958-1','Tyco',0,'',0,18,'10',NULL,NULL),(3,'XP-3','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','Wago 770-853/011-000','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','WAGO Kontakttechnik GmbH & Co. KG','770-853/011-000','770-853/011-000','','20 А','600 В','','-35','85','','',2001,'2014-02-28 14:36:30','770-853/011-000','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wago\\wago 770-853_011-000.pdf',6,0,'Розетка угловая на плату 770-853/011-000','Wago',0,'',0,18,'20',NULL,NULL),(4,'XP-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','TE 223975-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','223975-1','223975-1','','10 А','250 В','','-55','125','','',2001,'2014-02-28 14:37:03','223975-1','\\\\FSERVER\\Pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_223975_E.pdf',3,0,'Вилка угловая на плату UPM 223975-1','Tyco',0,'',0,18,'10',NULL,NULL),(5,'Connect-40','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','5177983-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','5177983-1','5177983','','0,5 А','100 В','','-40','+125','','',2001,'2014-04-14 13:29:15','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_5177983_B1.pdf',4,0,'Розетка прямая на плату 5177983-1','Tyco',0,'',1,18,'0,5',NULL,NULL),(6,'Connect-37','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','TE 5747847-4','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','5747847-4','5747847','5747847-4','0,5 А','100 В','','-40','+125','','',2008,'2014-02-28 14:35:34','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_5747847_A1.pdf',2,0,'Розетка угловая на плату 5747847-4','Tyco',0,'',0,18,'0,5',NULL,NULL),(7,'Connect-55S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','352346-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','352346-1','352346','','1,5 А','500 В','','-40','+125','','',2008,'2014-07-15 08:58:48','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_DS_65911_0705.pdf',4,0,'Розетка угловая на плату 352346-1','Tyco',0,'',0,18,'1,5',NULL,NULL),(8,'USB','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','USBB-1J','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','292304-1','USBB-1J','','1 А','30 В','','-40','+125','','',2008,'2014-02-28 14:35:18','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\tyco~292304-1.pdf',2,0,'292304-1','Разъем на плату USBB-1J угловой',0,'USBB-1J',0,18,'1',NULL,NULL),(9,'XP-4','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','B4B-PH-K-S','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Вилка на плату B4B-PH-K-S','B4B-PH-K-S','','2 А','100 В','','-25','+85','','',2008,'2014-02-28 14:35:04','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\ePH.pdf',3,0,'Вилка на плату B4B-P','JST',0,'Вилка на плату B4B-P',0,18,'2',NULL,NULL),(10,'XP-1','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Штыревой разъем на плату PLS-1','PLS-1','','3 А','500 В','','-40','+125','','',2008,'2014-03-04 11:35:47','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\DOC000227157.pdf',1,0,'Штыревой разъем на плату PLS-1','',0,'Штыревой разъем на п',0,18,'3',NULL,NULL),(11,'Connect-40','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','5-5179009-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','5-5179009-1','5179009','','0,5 А','100 В','','-40','+125','','',2001,'2014-03-06 17:02:15','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_5179009_P.pdf',1,0,'Розетка прямая на плату 5-5179009-1','Tyco',0,'',1,18,'0,5',NULL,NULL),(12,'XS-2+BAT','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','CH-74-2032','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Батарейный отсек вертикальный с батарейкой CR2032','CH-74-2032','','3 А','500 В','','-40','+105','','',2008,'2014-04-08 17:39:05','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Батарейный отсек CH-74-2032.pdf',2,0,'Батарейный отсек вер','CH-74-2032',0,'Батарейный отсек вер',0,18,'3',NULL,NULL),(13,'mini/microUSB','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','1734753-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','1734753-1','1734753','','1 А','30 В','','-40','+125','','',2008,'2014-04-03 11:28:53','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_1734753_A.pdf',1,0,'Разъем на плату miniUSB-B прямой 1734753-1','Tyco',0,'',0,18,'1',NULL,NULL),(14,'Connect-55P1','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','106081-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','106081-1','TYPE C','','55 поз','1,5 А','','-55','+125','','',2001,'2014-07-15 08:57:39','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_106081_J.pdf',3,0,'Вилка прямая на плату 55 конт. 106081-1','Tyco',0,'',0,27,'55',NULL,NULL),(15,'XS-3','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','5223955-2','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','5223955-2','UPM 3','','3 поз','10 А','','-55','+125','','',2001,'2014-04-11 10:30:28','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_5223955_P.pdf',1,0,'Розетка прямая на плату 3 поз. 5223955-2','Tyco',0,'',0,27,'3',NULL,NULL),(16,'XS-8','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','120953-5','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','120953-5','UPM 8','','8 поз','10 А','','-55','+125','','',2001,'2014-04-11 10:36:23','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_120953_B1.pdf',1,0,'Розетка прямая на плату 8 поз. 120953-5','Tyco',0,'',0,27,'8',NULL,NULL),(17,'Connect-40','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','5177984-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','5177984-1','5177984-1','','40 поз','0.5 А','','-40','+125','','',2001,'2014-04-14 13:28:58','100 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_5177984_A2.pdf',1,0,'Вилка прямая на плату 5177984-1','Tyco',0,'',1,27,'40',NULL,NULL),(18,'XP-2 + jumper','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS-2','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Штыревой разъем на плату PLS-2 с джампером MJ-O-6','PLS-2','','3 А','500 В','','-40','125','','',2008,'2014-04-18 14:42:07','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\ec278.pdf',1,0,'Штыревой разъем на плату PLS-2 с джампером MJ-O-6','',0,'Штыревой разъем на п',0,18,'3',NULL,NULL),(19,'Connect-64S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBD-64','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Гнездо на плату PBD-64 2,54мм','PBD-64','','64 поз','3 А','','-55','140','','',2008,'2014-07-15 08:56:48','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\ec278.pdf',2,0,'Гнездо на плату PBD-','',0,'Гнездо на плату PBD-',0,27,'64',NULL,NULL),(20,'Connect-40S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBD-40','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Гнездо на плату PBD-40 2,54мм','PBD-40','','40 поз','3 А','','-55','140','','',2008,'2014-07-15 08:56:37','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\ec278.pdf',3,0,'Гнездо на плату PBD-','',0,'Гнездо на плату PBD-',0,27,'40',NULL,NULL),(21,'XT-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MKDSO 2,5/2-L','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','MKDSO 2,5/2-L','','','2 поз','24 А','','-40','+85','','',2008,'2014-05-20 17:14:41','630 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\ItemDetail_1707205.pdf',1,0,'Клеммник на плату угловой левый MKDSO 2,5/2-L','Dinkle',0,'',0,27,'2',NULL,NULL),(22,'XT-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MKDSO 2,5/2-R','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','MKDSO2,5/ 2-R','','','2 поз','24 А','','-40','+85','','',2008,'2014-05-20 17:16:29','630 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\ItemDetail_1707195.pdf',1,0,'Клеммник на плату угловой правый MKDSO 2,5/ 2-R','Dinkle',0,'',0,27,'2',NULL,NULL),(23,'Connect-20S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','BHS2-20','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Чип вилка на плату BHS2-20','BH2-20','','20 шт','1 А','','-40','+105','','',2001,'2014-07-15 08:56:12','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\IDC.pdf',2,0,'Чип вилка на плату BHS2-20','',0,'Чип вилка на плату B',1,26,'20',NULL,NULL),(24,'XP-4x2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MDSTBVA 2,5/ 4-G','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','MDSTBVA 2,5/ 4-G','MDSTBVA 2,5/ 4-','','10 А','250 В','','-40','+85','','',2008,'2014-05-26 16:34:31','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\MDSTBVA 2,5 4-G.pdf',1,0,'Вилка на плату MDSTBVA 2,5/ 4-G','Phoenix Contact',0,'',0,18,'10',NULL,NULL),(25,'XP-2x2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MDSTBVA 2,5/ 2-G','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','MDSTBVA 2,5/2-G','MDSTBVA 2,5/2-G','','10 А','250 В','','-40','+85','','',2008,'2014-05-26 16:37:34','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\MDSTBVA 2,52-G.pdf',1,0,'Вилка на плату MDSTBVA 2,5/2-G','Phoenix Contact',0,'',0,18,'10',NULL,NULL),(26,'Connect-2P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','231-132','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','WAGO Kontakttechnik GmbH & Co. KG','231-132','231-132','','12 А','320 В','','-40','+85','','',2008,'2014-05-26 16:42:50','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wago\\231-132_001-000.pdf',1,0,'Вилка на плату 231-132','Wago',0,'',0,18,'12',NULL,NULL),(27,'Connect-9','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MCV 1,5/9-G-3,5','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','MCV 1,5/ 9-G-3,5','MCV 1,5/ 9-G-3,','','8 А','160 В','','-40','+85','','',2008,'2014-05-26 17:38:06','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\MCV 1,5 9-G-3,5.pdf',1,0,'Вилка на плату MCV 1,5/ 9-G-3,5','Phoenix Contact',0,'',0,18,'8',NULL,NULL),(28,'XT-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MKDS 3_2','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','MKDS 3/2','','','2 поз','250 В','','-40','+85','','',2008,'2014-07-15 09:32:27','24 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\MKDS 3 2 - 1711026.pdf',4,0,'Винтовые клемы шаг 5мм MKDS 3/2','Phoenix Contact',0,'',0,27,'2',NULL,NULL),(29,'XT-8','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MKDS 3_8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','MKDS 3/8','','','8 поз','250 В','','-40','+85','','',2008,'2014-07-15 09:32:38','24 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\MKDS 3 8 - 1711084.pdf',2,0,'Клеммник винт 8 поз. 5мм MKDS 3/8','Phoenix Contact',0,'',0,27,'8',NULL,NULL),(30,'Connect-3','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MKDS 3_3','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','MKDS 3/3','','','3 поз','250 В','','-40','+85','','',2008,'2014-06-05 11:44:41','24 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\MKDS 3 2 - 1711026.pdf',1,0,'Винтовые клемы шаг 5мм MKDS 3/3','Phoenix Contact',0,'',0,27,'3',NULL,NULL),(31,'Connect-6','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','WM SL 5.08HC/06','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Weidmuller','SL 5.08HC/06/90LF','SL 5.08HC/06/90','','10 А','300 В','','-40','+85','','',2008,'2014-06-30 16:20:49','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Weidmuller\\1150390000_SL_5.08HC_06_90LF_3.2SN_BK_BX_ru.pdf',1,0,'Вилка на плату SL 5.08HC/06/90LF','Weidmuller1150390000',0,'',0,18,'10',NULL,NULL),(32,'Connect-6','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','WAGO 769-666/003-000','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','WAGO Kontakttechnik GmbH & Co. KG','769-666/003-000','769-666/003-000','','6 поз','250 В','','-40','+85','','',2008,'2014-07-02 18:34:02','10 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wago\\wago 769-666_003-000.pdf',1,0,'Вилка на плату 6 поз. WAGO 769-666/003-000','WAGO',0,'',0,27,'6',NULL,NULL),(33,'Connect-40P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLD-40','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Вилка на плату PLD-40','PLD-40','','40 поз','3 А','','-55','140','','',2001,'2014-07-04 16:59:53','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\PLD2-40.pdf',2,0,'Вилка на плату PLD-40','',0,'Вилка на плату PLD-4',0,27,'40',NULL,NULL),(34,'Connect-64P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLD-64','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Вилка на плату PLD-64','PLD-64','','64 поз','3 А','','-55','140','','',2001,'2014-07-04 16:59:26','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\PLD2-40.pdf',1,0,'Вилка на плату PLD-64','',0,'Вилка на плату PLD-6',0,27,'64',NULL,NULL),(35,'Connect-10P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS2-10R','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Вилка на плату PLS2-10R','PLS2-10R','','10 поз','2 А','','-55','105','','',2008,'2014-07-07 13:13:56','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\PLS2-10R.pdf',1,0,'Вилка на плату PLS2-10R','',0,'Вилка на плату PLS2-',0,27,'10',NULL,NULL),(36,'Connect-10S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBS2-10','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Гнездо на плату PBS2-10','PBS2-10','','10 поз','1 А','','-40','105','','',2008,'2014-07-15 08:55:27','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\PBS2-10.pdf',3,0,'Гнездо на плату PBS2','',0,'Гнездо на плату PBS2',0,27,'10',NULL,NULL),(37,'Connect-6','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','SV 7.62HP-06-90MSF','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Weidmuller','SV 7.62HP/06/90MSF','SV 7.62HP/06/90','SV 7,62','6 поз','35 А','','-40','125','','',2008,'2014-07-09 16:10:18','300 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Weidmuller\\1048770000_SV_7.62HP_06_90MSF_3.5SN_BK_BX_ru.pdf',4,0,'Вилка угловая на плату SV 7.62HP/06/90MSF','1048740000',0,'',0,27,'6',NULL,NULL),(38,'Connect-8P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS2-8R','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','PLS2-8R','','','8 поз','','','','','','',2010,'2014-07-09 14:01:41','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\PLS2-10R.pdf',1,0,'PLS2-8R','Вилка на плату угловая однорядная 2мм',0,'',0,27,'8',NULL,NULL),(39,'Connect-15P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS2-15R','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','PLS2-15R','','','15 поз','','','','','','',2010,'2014-07-09 15:41:05','','E:\\TSFASMAN\\АВМ-ИЭ\\v1.0\\components PDF\\PLS2-10R.pdf',1,1,'Штыри на плату шаг 2мм 15 контактов прямой угол','',0,'',0,27,'15',NULL,NULL),(40,'Connect-15S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBS2-15','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','PBS2-15','','','15 поз','','','','','','',2010,'2014-07-15 08:54:57','','E:TSFASMANАВМ-ИЭv1.0components PDFPBS2-10.pdf',2,1,'Гнезда на плату шаг 2мм 15 контактов прямые','',0,'',0,27,'15',NULL,NULL),(41,'Connect-6','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','SV 7.62HP-06-90G','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Weidmuller','SV 7,62HP/06/90G','','','6 поз','','','','','','',2010,'2014-07-10 17:36:23','','E:\\Components\\weidmuller\\1930310000_SV_7.62HP_06_90G_3.5SN_BK_BX_en.pdf',1,1,'Вилка OMNIMATE POWER на плату SV 7.62/06/90G 3.5SN BK BX','1930310000',0,'',0,27,'6',NULL,NULL),(42,'XP-3','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','Wago 770-813/011-000','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','WAGO Kontakttechnik GmbH & Co. KG','770-803/011-000','770-803/011-000','','20 А','600 В','','-35','85','','',2001,'2014-07-14 14:12:32','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wago\\770-803_011-000.pdf',1,0,'Вилка угловая на плату 770-803/011-000','Wago',0,'',0,18,'20',NULL,NULL),(43,'Connect-2P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MF-02MB','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Вилка на плату MF-02MB','Mini-Fit','','2 поз','8 А','','-25','+85','','',2001,'2014-07-15 12:54:12','600 В AC','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\mini-fit.pdf',1,0,'Вилка на плату MF-02MB','1586038-2',0,'Вилка на плату MF-02',0,27,'2',NULL,NULL),(44,'Connect-4P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MF-04MB','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Вилка на плату MF-04','Mini-Fit','','4 поз','8 А','','-25','+85','','',2001,'2014-07-15 12:55:15','600 В AC','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\mini-fit.pdf',1,0,'Вилка на плату MF-04','1586495-4',0,'Вилка на плату MF-04',0,27,'4',NULL,NULL),(45,'Connect-8','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','6368150','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','6368150','RJ-45','','8P8C поз','cat 5','','-40','+85','','',2001,'2014-07-15 14:07:52','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_6368150_C1.pdf',1,0,'Розетка прямая на плату RJ-45 8P8C (6368150)','Tyco',0,'',0,27,'8P8C',NULL,NULL),(46,'Connect-4P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','SV 7.62HP-04-90G','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Weidmuller','SV 7,62HP/04/90G','','','4 поз','','','','','','',2010,'2014-07-18 14:12:33','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Weidmuller\\1026760000_SL_7.62HP_02_90G_3.2SN_BK_BX_ru.pdf',1,1,'Вилка OMNIMATE POWER на плату SV 7.62/06/90G 3.5SN BK BX','1930310000',0,'',0,27,'4',NULL,NULL),(47,'Connect-4P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','SV 7.62HP/04/90MSF3','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Weidmuller','SV 7.62HP/04/90MSF3','SV 7.62HP/06/90','SV 7,62','4 поз','35 А','','-40','125','','',2010,'2014-07-22 11:58:11','300 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Weidmuller\\1048580000_SV_7.62HP_04_90MSF3_3.5SN_BK_BX_ru.pdf',1,0,'Вилка угловая на плату SV 7.62HP/04/90MSF3','1048580000',0,'',0,27,'4',NULL,NULL),(48,'mini/microUSB','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','MOLEX 500075-0517','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Molex','500075-0517','','','1 А','30 В','','-40','125','','',2008,'2014-07-23 18:28:19','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Molex 50075-0517.pdf',2,0,'500075-0517','Разъем на плату miniUSB-B прямой',0,'',0,18,'1',NULL,NULL),(49,'Connect-5','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBS2-5','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','Гнездо на плату PBS2-5','','','5 поз','','','','','','',2010,'2014-07-24 17:57:49','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\PBS2-10.pdf',1,1,'Гнездо на плату PBS2-10','',0,'',0,27,'5',NULL,NULL),(50,'Connect-5P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS2-5R','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','Вилка угловая на плату PLS2-5R','','','5 поз','','','','','','',2010,'2014-07-24 18:08:41','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\PLS2-10R.pdf',1,1,'Вилка угловая на плату PLS2-5R','',0,'',0,27,'5',NULL,NULL),(51,'Connect-37','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','TE 5747462-4','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','5747462-4','5747642','5747462-4','37 поз','1000 В','','-40','125','','',2008,'2014-08-01 09:42:52','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_5747462_O.pdf',3,0,'Розетка угловая на плату 5747462-4','Tyco',0,'',0,27,'37',NULL,NULL),(52,'XS4','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBS-4','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','PBS-4','','','4 поз','','','','','','',2010,'2014-08-08 12:12:55','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\PBS.pdf',1,1,'Гнездо на плату прямое шаг 2,54мм PBS-4','',0,'',0,27,'4',NULL,NULL),(53,'XP-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS-2w-outJumper','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Штыревой разъем на плату PLS-2','PLS-2','','3 А','500 В','','-40','+125','','',2008,'2014-08-13 11:48:20','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\DOC000227157.pdf',1,0,'Штыревой разъем на плату PLS-2','',0,'Штыревой разъем на п',0,18,'3',NULL,NULL),(54,'XP-3 + jumper','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS-3w-outJumper','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','XP-3 + jumper','PLS-3','','2 А','500 В','','-40','125','','',2010,'2014-08-18 10:14:23','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\ec278.pdf',2,0,'Штыревой разъем на плату 3 поз с джампером MJ-O-6','',0,'Штыревой разъем на п',0,18,'2',NULL,NULL),(55,'Connect-10S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBD-10','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','PBD-10','','','10 поз','','','','','','',2010,'2014-09-05 17:19:44','','',1,0,'Гнездо на плату шаг 2,54 мм 10 контактов','',0,'',0,27,'10',NULL,NULL),(56,'Connect-10P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','DS1029-4-2*5 - duplicate','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','DS1029-4-2*5','','','10 поз','','','','','','',2010,'2014-09-05 17:28:53','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\ds1029-04.pdf',1,1,'Соединитель межплатный штыревой двухрядный 2*5 контактов 17мм','DS10-29-4-2*5-P8B-VB-43',0,'',0,27,'10',NULL,NULL),(57,'XP-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLS2-2','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Штыревой разъем на плату PLS2-2','PLS2-2','','2 поз','1 А','','-40','125','','',2008,'2014-10-15 10:18:15','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\113.pdf',1,0,'Штыревой разъем на плату PLS2-2','Штыревой разъем на плату PLS2-2',0,'Штыревой разъем на п',0,27,'2',NULL,NULL),(58,'XT-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','DG330-5.0-02','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Клеммник DG330-5.0-02','DG330-5.0-02','','2 поз','250 В','','-40','125','','',2008,'2014-12-09 13:18:55','15 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\dg330.pdf',1,0,'Клеммник DG330-5.0-02','',0,'Клеммник DG330-5.0-0',0,27,'2',NULL,NULL),(59,'XT-1','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','WAGO 257-501','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','WAGO Kontakttechnik GmbH & Co. KG','257-501','','','1 поз','400 В','','-40','125','','',2008,'2015-02-05 17:18:08','24 А','',1,0,'257-501','Wago',0,'',0,27,'1',NULL,NULL),(60,'Connect-8S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','TE 5555164-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Tyco Electronics','5555164-1','','','8 поз','','','-40','125','','',2008,'2015-04-20 16:03:31','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\TE\\ENG_CD_5555164_B1.pdf',1,0,'Розетка RJ-45 угловая на плату 5555164-1','TE',0,'',0,27,'8',NULL,NULL),(61,'XS-3','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','DS210','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Разъем питания круглый d=2.1 мм DS-210','','','3 поз','','','-40','85','','',2008,'2015-04-20 16:46:54','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\DS-210.pdf',1,0,'Разъем питания круглый d=2.1 мм DS-210','DS210',0,'Разъем питания кругл',0,27,'3',NULL,NULL),(62,'Connect-16S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','IDC-16MR','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Розетка угловая на плату IDC-16MR','','','16 поз','','','-80','125','','',2008,'2015-04-23 15:29:22','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\IDC.pdf',1,0,'Розетка угловая на плату IDC-16MR','',0,'Розетка угловая на п',0,27,'16',NULL,NULL),(63,'XP-40','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','WR-PHD40 2.54','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Wurth Elektronik','61306421221','PHD-40','','40 поз','3 А','','-40','125','','',2001,'2015-07-06 11:35:28','250 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wurth Elektronik\\6130xx2xx21.pdf',4,0,'Штыри на плату WR-PHD40 61306421221','Wurth',0,'Штыри на плату WR-PH',0,27,'40',NULL,NULL),(64,'Connect-10S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBD2-10','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','CONNFLY ELECTRONIC CO,.LTD.','Гнездо на плату PBD2-10','','','10 поз','','','0','85','','',2001,'2015-05-13 13:24:23','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Connfly\\pbd21026-05.pdf',3,0,'Гнездо на плату PBD2-10','DS1026-05-2*5S8BV',0,'',0,27,'10',NULL,NULL),(65,'Connect-8S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBD2-8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','CONNFLY ELECTRONIC CO,.LTD.','Гнездо на плату PBD2-8','','','8 поз','','','0','85','','',2001,'2015-05-13 13:25:03','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Connfly\\pbd21026-05.pdf',1,0,'Гнездо на плату PBD2-8','DS1026-05-2*4S8BV',0,'',0,27,'8',NULL,NULL),(66,'Connect-20S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBD2-20','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','CONNFLY ELECTRONIC CO,.LTD.','Гнездо на плату PBD2-20','','','20 поз','','','0','85','','',2001,'2015-05-13 13:25:41','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Connfly\\pbd21026-05.pdf',1,0,'Гнездо на плату PBD2-20','DS1026-05-2*10S8BV',0,'',0,27,'20',NULL,NULL),(67,'2Eth+LED','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','615016245421','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Wurth Elektronik','615016245421','615016245421','','2 поз','120 В','','-40','85','','',2001,'2015-06-08 11:22:41','1,5 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wurth Elektronik\\615016245421.pdf',2,0,'Двойная розетка Ethernet на плату 615016245421','Wurth Elektronik',0,'',0,27,'2',NULL,NULL),(69,'Connect-2x6P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','WAGO 734-406/001-000','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','WAGO Kontakttechnik GmbH & Co. KG','734-406/001-000','734-406','734-406','12 поз','250 В','','-60','+100','','',2001,'2015-06-08 11:28:05','10 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wago\\734-406_001-000.pdf',1,0,'Вилка угловая на плату 734-406/001-000','Wago',0,'',0,27,'12',NULL,NULL),(68,'Eth+2USB','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','615016338621','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Wurth Elektronik','615016338621','615016338621','','3 шт','120 В','','-20','+85','','',2001,'2015-06-08 11:22:19','1,5 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wurth Elektronik\\615016338621.pdf',2,0,'Розетка Ethernet+2USB на плату 615016338621','Wurth Elektronik',0,'',0,26,'3',NULL,NULL),(70,'USB2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','61400826021','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Wurth Elektronik','61400826021','61400826021','','2 шт','120 В','','-20','+85','','',2001,'2015-06-26 15:06:17','1,5 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Wurth Elektronik\\61400826021.pdf',1,0,'Розетка двойная USB на плату 615016338621','Wurth Elektronik',0,'',0,26,'2',NULL,NULL),(71,'XP-4','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','DS1029-01-1*4P8BV1 (H=12mm)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','CONNFLY ELECTRONIC CO,.LTD.','DS1029-01-1*4P8BV1','PLH-4','','4 поз','600 В','','-40','+105','','',2001,'2015-07-06 11:20:34','1,5 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Connfly\\DS1029-01.pdf',2,0,'Соединитель межплатный штыревой 1*4 12мм','Connfly',0,'',0,27,'4',NULL,NULL),(72,'Connect-24P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','Phoenix DMC 1,5/12-G1F-3,5-LR P20THR','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Phoenix Contact','DMC 1,512-G1F-3,5-LR P20THR','','','250 В','8 А','','-40','85','','',2008,'2015-09-21 11:59:16','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Phoenix\\Phoenix DMC 1,512-G1F-3,5-LR P20THR.pdf',1,0,'Разъем на плату Phoenix DMC 1,512-G1F-3,5-LR P20THR','',0,'',0,14,'250',NULL,NULL),(73,'Connect-20S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBD-20','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','НКП','Розетка на плату PBD-20 шаг 2,54 мм','PBD-20','','20 поз','3 А','','-40','105','','',2008,'2015-09-29 11:19:59','500 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\ec278.pdf',1,0,'Розетка на плату PBD-20 шаг 2,54 мм','Розетка на плату PBD-20 с шагом 2,54',0,'Розетка на плату PBD',0,27,'20',NULL,NULL),(74,'Connect-50S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PBD-50','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','PBD-50','','',' шт','','','','','','',2010,'2016-01-22 12:22:08','','',1,1,'Гнездо на плату прямая 2,54 мм  PBD-50','',0,'',0,26,'',NULL,NULL),(75,'Connect-10P','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','PLD-10','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Amphenol','Вилка на плату прямая PLD-10 2x5 шаг 2,54 мм','PLD-10','XP?',' шт','','','','','','',2010,'2016-01-27 10:30:21','','',1,1,'Вилка прямая PLD-10 2x5 шаг 2,54 мм','',0,'',0,26,'',NULL,NULL),(76,'Connect-55S','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\connectors.SchLib','Harting 17 23 055 1101','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\connectors.PcbLib','','','','','Harting','17 23 055 1101','17 23 055 1101','','55 поз','500 В','','-40','125','','',2008,'2016-02-05 18:39:48','1 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Connectors\\Harting\\1723055X101_100119999DRW003A.PDF',1,0,'Розетка угловая на плату 17 23 055 1101','Harting',0,'',0,27,'55',NULL,NULL);
/*!40000 ALTER TABLE `connectors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `description`
--

DROP TABLE IF EXISTS `description`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `description` (
  `iddescription` int(11) NOT NULL,
  `description` varchar(64) DEFAULT NULL,
  `descriptionfull` varchar(64) DEFAULT NULL,
  `url` varchar(128) DEFAULT NULL,
  `deleted` varchar(1) NOT NULL DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddescription`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `description`
--

LOCK TABLES `description` WRITE;
/*!40000 ALTER TABLE `description` DISABLE KEYS */;
INSERT INTO `description` VALUES (1,'capasitors','Конденсаторы','/','0',NULL,NULL),(2,'connections','Соединения','/','0',NULL,NULL),(3,'connectors','Разъёмы','/','0',NULL,NULL),(4,'diodes','Диоды','/','0',NULL,NULL),(5,'gdts','Разрядники','/','0',NULL,NULL),(6,'ic_adcdac','М/сх АЦП и ЦАП','/ic/','0',NULL,NULL),(7,'ic_anmixed','М/сх аналоговые','/ic/','0',NULL,NULL),(8,'ic_answitch','М/сх аналоговые ключи','/ic/','0',NULL,NULL),(9,'ic_interface','М/сх интерфейсные','/ic/','0',NULL,NULL),(10,'ic_isolator','М/сх изоляторы','/ic/','0',NULL,NULL),(11,'ic_microcontroller','М/сх микроконтроллеры','/ic/','0',NULL,NULL),(12,'ic_memory','М/сх память','/ic/','0',NULL,NULL),(13,'ic_opamp','М/сх операционные усилители','/ic/','0',NULL,NULL),(14,'ic_opto','М/сх оптоэлектроника','/ic/','0',NULL,NULL),(15,'ic_powermgmt','М/сх управления питанием','/ic/','0',NULL,NULL),(16,'ic_protection','М/сх защитные','/ic/','0',NULL,NULL),(17,'ic_vreference','М/сх источников напряжения','/ic/','0',NULL,NULL),(18,'inductors','Индуктивности','/','0',NULL,NULL),(19,'mechanical','Механические компоненты','/','0',NULL,NULL),(20,'modules','Модули','/','0',NULL,NULL),(21,'opto','Оптоэлектроника','/','0',NULL,NULL),(22,'oscillators','Генераторы, кварцы','/','0',NULL,NULL),(23,'posistors','Предохранители самовосстанавливающиеся','/','0',NULL,NULL),(24,'power','Источники питания','/','0',NULL,NULL),(25,'qaltium','Карантин','/','0',NULL,NULL),(26,'resistors','Резисторы','/','0',NULL,NULL),(27,'switches','Устройства коммутации','/','0',NULL,NULL),(28,'symbols','Символы','/','0',NULL,NULL),(29,'transform','Трансформаторы','/','0',NULL,NULL),(30,'transistors','Транзисторы','/','0',NULL,NULL),(31,'tvs','Защитные диоды','/','0',NULL,NULL),(32,'varistors','Варисторы','/','0',NULL,NULL),(33,'ic_logic','М/сх логика','/ic/','0','2001','2015-11-04 21:16:37'),(34,'relay','Реле','/','0','2001','2015-11-04 21:07:21');
/*!40000 ALTER TABLE `description` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `diodes`
--

DROP TABLE IF EXISTS `diodes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `diodes` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `diodes`
--

LOCK TABLES `diodes` WRITE;
/*!40000 ALTER TABLE `diodes` DISABLE KEYS */;
INSERT INTO `diodes` VALUES (1,'Diode','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','ST Microelectronics','STTH208U','DO-214AA','U08','2 А','1.65 В','','-50','175','2.5 Вт','25 С/Вт',0,'2014-02-28 14:38:47','STTH208U','\\\\FSERVER\\PCAD\\Altium\\Libs\\DSheet\\Diodes\\ST\\stth208.pdf',3,0,'Диод STTH208U','ST',0,'',1,18,'2',NULL,NULL),(2,'Diode_Schottky','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','ON Semiconductor','MBRS1100T3G','DO-214AA','B1C','1 А','0.75 В','','-65','175','','22 С/Вт',2001,'2014-02-28 14:38:25','MBRS1100T3G','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\On Semiconductor\\MBRS1100T3-D.pdf',3,0,'Диод Шоттки MBRS1100T3','VS-10BQ100TR,SBR1U150SA',0,'',1,18,'1',NULL,NULL),(7,'2Diodes','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','SOT23','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','Fairchild Semiconductor','BAV99','SOT23','A7','0.85 В','70 В','','-55','+150','0.35 Вт','',2008,'2014-02-28 14:38:01','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\Fairchild\\BAV99.pdf',2,0,'Диодная сборка BAV99','Fairchild',0,'',1,14,'0.85',NULL,NULL),(8,'Diode_Schottky','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','DO-214AB (SMC)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','ST Microelectronics','STPS3L60S','DO-214AB (SMC)','S36','3 А','60 В','','-40','+125','1,6 Вт','',2001,'2014-02-28 14:37:43','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\ST\\stps3l60s.pdf',2,0,'Диод Шоттки STPS3L60S','ST',0,'',1,18,'3',NULL,NULL),(9,'2DiodesComCat','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','SOT23','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','Fairchild Semiconductor','BAT54C','SOT23','L43','400 мВ','30 В','','-55','150','0,29 Вт','430 C/Вт',2008,'2014-04-02 18:36:09','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\Fairchild\\BAT54.pdf',1,0,'Диодная сборка BAT54C','Fairchild',0,'',1,15,'400',NULL,NULL),(10,'Zener','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','DO-201AE','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','ON Semiconductor','1N5378B','DO-201AE','1N5378B','100 В','12 мА','','-65','+120','5 Вт','',2008,'2014-05-23 18:04:52','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\On Semiconductor\\1N53-b.pdf',1,0,'1N5378B','On Semiconductor',0,'',0,14,'100',NULL,NULL),(11,'Zener','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','ON Semiconductor','1SMB5949BT3','DO-214AA','949B','100 В','3,7 мА','5%','-65','150','3 Вт','',2008,'2014-06-10 11:03:54','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\On Semiconductor\\1smb59-b.pdf',1,0,'1SMB5949BT3','ON Semiconductor',0,'',1,14,'100',NULL,NULL),(12,'Diode','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','SOD323F','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','Fairchild Semiconductor','1N4148WS','SOD-323F','','150 мА','1 В','','-55','155','0,2 Вт','',2008,'2014-07-17 18:10:53','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\Fairchild\\1N4148WS.pdf',1,0,'1N4148WS','Fairchild',0,'',1,19,'150',NULL,NULL),(13,'2DiodesComAn','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','SOT23','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','Fairchild Semiconductor','BAT54A','SOT-23','L42','400 мВ','30 В','','-55','150','0,29 Вт','430 С/Вт',2008,'2014-10-16 15:41:44','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\Fairchild\\BAT54.pdf',1,0,'Диодная сборка BAT54A','Fairchild',0,'',1,15,'400',NULL,NULL),(14,'Diode (1-3)','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','SOT23','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','Fairchild Semiconductor','MMBD4148','SOT-23','5H','1 В','200 мА','','-50','+125','0,35 Вт','357 C/W',2001,'2015-06-16 17:18:32','75 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\Fairchild\\MMBD4148.pdf',1,0,'Малосигнальный диод MMBD4148','Fairchild',0,'',1,14,'1',NULL,NULL),(15,'Zener','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\diodes.SchLib','SOD323','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\diodes.PcbLib','','','','','NXP','PDZ36B.115','SOD-323','ZU','36 В','50 нА','','-40','150','0,4 Вт','',2008,'2015-09-22 11:16:10','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Diodes\\NXP\\nxp-pdz-b.pdf',1,0,'PDZ36B.115','NXP',0,'',1,14,'36',NULL,NULL);
/*!40000 ALTER TABLE `diodes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gdts`
--

DROP TABLE IF EXISTS `gdts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gdts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(10) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gdts`
--

LOCK TABLES `gdts` WRITE;
/*!40000 ALTER TABLE `gdts` DISABLE KEYS */;
INSERT INTO `gdts` VALUES (5,'GDT','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\GDTs.SchLib','Bourns 2035','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\GDTs.PcbLib','','','','','Bourns Inc','2035-09-SM-RPLF','2035','','90 В','','','-55','+85','','',2008,'2014-05-20 14:49:55','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\GDTs\\2035-sm.pdf',1,0,'Газоразрядная трубка  Bourns 2035-09-SM-RPLF','Bourns',0,'',1,'90',14,NULL,NULL),(6,'2GDT','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\GDTs.SchLib','BOURNS_2038','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\GDTs.PcbLib','','','','','Bourns Inc','2038-15-SM','2038','','150 В','70 В','','-40','+90','','',2001,'2015-06-17 13:12:18','5 кА','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\GDTs\\2038-xx-SM.pdf',1,0,'Разрядник 2038-15-SM','Bourns',0,'',1,'150',14,NULL,NULL);
/*!40000 ALTER TABLE `gdts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_adcdac`
--

DROP TABLE IF EXISTS `ic_adcdac`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_adcdac` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(10) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_adcdac`
--

LOCK TABLES `ic_adcdac` WRITE;
/*!40000 ALTER TABLE `ic_adcdac` DISABLE KEYS */;
INSERT INTO `ic_adcdac` VALUES (5,'AD73360','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_ADCDAC.SchLib','SOIC-28','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_ADCDAC.PcbLib','','','','','Analog Devices','AD73360AR','SO-28','','16 бит','64кС/с','77 дБ','-40','+85','','',2008,'2014-06-27 11:08:17','1,25;2,5 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\ADCDAC\\Analog Devices\\AD73360.pdf',1,0,'AD73360AR','16бит Сиг-Дел АЦП AD73360AR',0,'',1,'16',0,NULL,NULL),(6,'MCP3903','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_ADCDAC.SchLib','SSOP-28','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_ADCDAC.PcbLib','','','','','Microchip','АЦП МСР3903 E/SS','ssop-28','MCP3903',' ','','','','','','',2010,'2016-01-22 15:24:34','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\ADCDAC\\Microchip\\АЦП MCP3903.pdf',1,0,'АЦП МСР3903 E/SS','',0,'',1,'',0,NULL,NULL),(7,'MCP3913','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_ADCDAC.SchLib','SSOP-28','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_ADCDAC.PcbLib','','','','','Microchip','АЦП МСР3913 E/SS','ssop-28','MCP3903','24 bit ','','','','','','',2010,'2016-02-04 13:51:01','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\ADCDAC\\Microchip\\АЦП MCP3913.pdf',1,0,'АЦП МСР3903 E/SS','',0,'',1,'24 bit',0,NULL,NULL);
/*!40000 ALTER TABLE `ic_adcdac` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_anmixed`
--

DROP TABLE IF EXISTS `ic_anmixed`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_anmixed` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(10) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_anmixed`
--

LOCK TABLES `ic_anmixed` WRITE;
/*!40000 ALTER TABLE `ic_anmixed` DISABLE KEYS */;
INSERT INTO `ic_anmixed` VALUES (5,'74HC4046A','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_AnMixed.SchLib','SOIC-16','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_AnMixed.PcbLib','','','','','NXP','74HC4046AD','SO-16','','19 МГц','5 В','','-40','+85','','',2008,'2014-06-27 10:11:22','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\AnMixed\\Analog Devices\\74HC_HCT4046.pdf',2,0,'74HC4046AD','ФАПЧ с ГУН 74HC4046AD',0,'',1,'19',0,NULL,NULL),(6,'XTR105','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_AnMixed.SchLib','SO-14','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_AnMixed.PcbLib','','','','','Texas Instruments','XTR105U','SO-14','','7,5-36 В','2х0,8 мА','','-40','+85','','',2008,'2014-08-13 10:30:29','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\AnMixed\\TI\\xtr105.pdf',1,0,'Передатчик тока 4-20мА XTR105U','TI',0,'',1,'7,5-36',14,NULL,NULL);
/*!40000 ALTER TABLE `ic_anmixed` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_answitch`
--

DROP TABLE IF EXISTS `ic_answitch`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_answitch` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_answitch`
--

LOCK TABLES `ic_answitch` WRITE;
/*!40000 ALTER TABLE `ic_answitch` DISABLE KEYS */;
INSERT INTO `ic_answitch` VALUES (5,'ADG706','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_AnSwitch.SchLib','TSSOP-28','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_AnSwitch.PcbLib','','','','','Analog Devices','ADG706BRU','TSSOP28','ADG706','16 кан','7 В','','-40','+85','0,66 Вт','',2001,'2014-02-28 14:55:04','2,5 Ом','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\AnSwitch\\ADG706_707.pdf',2,0,'ADG706BRU','AD',0,'',1,28,NULL,NULL,'16');
/*!40000 ALTER TABLE `ic_answitch` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_interface`
--

DROP TABLE IF EXISTS `ic_interface`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_interface` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=11 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_interface`
--

LOCK TABLES `ic_interface` WRITE;
/*!40000 ALTER TABLE `ic_interface` DISABLE KEYS */;
INSERT INTO `ic_interface` VALUES (5,'KSZ8895xQ','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Interface.SchLib','PQFP-128','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Interface.PcbLib','','','','','Micrel','KSZ8895RQI','PQFP-128','KSZ8895RQ','100 МБит/с','3.3 В','','-40','+85','0.962','41.54 С/Вт',2001,'2014-04-08 15:48:57','KSZ8895RQI','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Interface\\ksz8895mq_rq_fmq_ds.pdf',5,0,'KSZ8895RQI','KSZ8895MQI',0,'',1,42,NULL,NULL,'100'),(6,'KSZ8081RNA','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Interface.SchLib','QFN-24','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Interface.PcbLib','','','','','Micrel','KSZ8081RNAI','QFN-24','KSZ8081RNA','100 МБит/с','3.3 В','','-40','85','0.155','49.22 С/Вт',2008,'2014-02-28 14:56:30','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Interface\\KSZ8081RNA-RND.pdf',2,0,'KSZ8081RNAI','Micrel',0,'',1,42,NULL,NULL,'100'),(7,'ST232','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Interface.SchLib','SOIC-16','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Interface.PcbLib','','','','','ST Microelectronics','ST232BDR','SO-16','ST232','220 кБит/с','RS-232','','-40','+85','','',2001,'2015-05-15 17:39:54','5 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Interface\\st232.pdf',1,0,'ST232BDR','ST',0,'',1,41,NULL,NULL,'220'),(8,'ST3485','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Interface.SchLib','SO-8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Interface.PcbLib','','','','','ST Microelectronics','ST3485EBDR','SO-8','ST3485','12 МБит/с','RS-485','','-40','+85','','',2001,'2015-05-15 17:46:32','3.3 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Interface\\st3485.pdf',2,0,'ST3485EBDR','ST',0,'',1,42,NULL,NULL,'12'),(9,'ST3485','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Interface.SchLib','SO-8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Interface.PcbLib','','','','','ST Microelectronics','ST485BDR','SO-8','ST485','12 МБит/с','RS-485','','-40','+85','','',2001,'2015-05-15 17:51:27','5 В','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Interface\\st3485.pdf',1,0,'ST485BDR','ST',0,'',1,42,NULL,NULL,'12'),(10,'ADM2483','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Interface.SchLib','WSOIC-16','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Interface.PcbLib','','','','','Analog Devices','ADM2483BRW','WSOIC-16','','500 кБит/с','RS-485','','-40','+85','','',2001,'2015-06-10 17:16:40','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Interface\\ADM2483.pdf',2,0,'ADM2483BRW','AD',0,'',1,41,NULL,NULL,'500');
/*!40000 ALTER TABLE `ic_interface` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_isolator`
--

DROP TABLE IF EXISTS `ic_isolator`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_isolator` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(10) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(45) DEFAULT NULL,
  `par5` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_isolator`
--

LOCK TABLES `ic_isolator` WRITE;
/*!40000 ALTER TABLE `ic_isolator` DISABLE KEYS */;
INSERT INTO `ic_isolator` VALUES (5,'ISO7640','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Isolator.SchLib','Wide So-16','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Isolator.PcbLib','','','','','Texas Instruments','ISO7640FM','SOIC16 DW','ISO7640FM',' 3/5V','','','-40','125','','',2010,'2014-07-15 13:52:42','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Isolator\\iso7640fm.pdf',3,0,'Изолятор цифровой четырехканальный ISO7640FM','',0,'',1,'',0,NULL,NULL);
/*!40000 ALTER TABLE `ic_isolator` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_logic`
--

DROP TABLE IF EXISTS `ic_logic`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_logic` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(10) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(45) DEFAULT NULL,
  `par5` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_logic`
--

LOCK TABLES `ic_logic` WRITE;
/*!40000 ALTER TABLE `ic_logic` DISABLE KEYS */;
INSERT INTO `ic_logic` VALUES (5,'74LVC1G123','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Logic.SchLib','MO-187-DA (SSOP-8)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Logic.PcbLib',NULL,NULL,NULL,NULL,'Texas Instruments','SN74LVC1G123DCT','MO-187-DA','C23Z',NULL,NULL,NULL,'-40','+125','0.57 Вт','220 C/W',2001,'2015-06-16 09:25:03',NULL,'\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Logic\\sn74lvc1g123.pdf',4,0,'Ждущий мультивибратор SN74LVC1G123DCT','TI',0,NULL,1,NULL,NULL,NULL,NULL),(6,'Schmitt Inverter (2-4)','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Logic.SchLib','SC70-5','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Logic.PcbLib',NULL,NULL,NULL,NULL,'ON Semiconductor','NL17SZ14DFT2G','SC70-5','LA M',NULL,'5 В',NULL,'-55','+125','0.186 Вт','350 C/W',2001,'2015-06-16 15:39:44',NULL,'\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Logic\\NL17SZ14-D.PDF',1,0,'Инвертер Шмитта NL17SZ14DFT2G','On Semi',0,NULL,1,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `ic_logic` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_memory`
--

DROP TABLE IF EXISTS `ic_memory`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_memory` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_memory`
--

LOCK TABLES `ic_memory` WRITE;
/*!40000 ALTER TABLE `ic_memory` DISABLE KEYS */;
INSERT INTO `ic_memory` VALUES (5,'AT25DF641','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Memory.SchLib','AT25DF641-S3H','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Memory.PcbLib','','','','','Atmel','AT25DF641-S3H','Wide SO-16','AT25DF641','8 Мб','4,1 В','','-55','125','','',2008,'2014-04-07 14:55:20','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Memory\\atmel~at25df641.pdf',3,0,'AT25DF641-S3H','Atmel',0,'',1,32,NULL,NULL,'8'),(6,'AT25DB081D','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Memory.SchLib','SO-8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Memory.PcbLib','','','','','Atmel','AT45DB081D-SU','SO-8','','1 Мб','3,6 В','','-55','125','','',2008,'2014-04-07 14:56:00','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Memory\\atmel~at45db081d.pdf',2,0,'Flash память 1 МБайт AT45DB081D-SU','Atmel',0,'',1,32,NULL,NULL,'1'),(7,'AT25DB081D','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Memory.SchLib','EIAJ SOIC_8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Memory.PcbLib','','','','','Atmel','AT45DB321D-SU','EIAJ SOIC-8','','4 Мб','3,6 В','','-55','125','','',2008,'2014-10-02 09:54:51','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Memory\\AT45DB321D.pdf',1,0,'Flash память 4 МБайта AT45DB321D-SU','Atmel',0,'',1,32,NULL,NULL,'4'),(8,'FM25CL64B','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Memory.SchLib','SO-8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Memory.PcbLib','','','','','Cypress','FM25CL64B-G','SO-8','','8 Кб','','','-40','+85','','',2010,'2014-11-19 09:52:08','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Memory\\FM25CL64B_001-84477_0D_V.pdf',1,0,'Память ферроэлектрическая 8Кх8 FM25CL64B-G','',0,'',1,31,NULL,NULL,'8');
/*!40000 ALTER TABLE `ic_memory` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_micrel`
--

DROP TABLE IF EXISTS `ic_micrel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_micrel` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_micrel`
--

LOCK TABLES `ic_micrel` WRITE;
/*!40000 ALTER TABLE `ic_micrel` DISABLE KEYS */;
INSERT INTO `ic_micrel` VALUES (5,'KSZ8895xQ','\\\\FSERVER\\PCAD\\Altium\\Libs\\Symbols\\IC\\IC_Micrel.SchLib','PQFP-128','\\\\FSERVER\\PCAD\\Altium\\Libs\\Footprints\\IC\\IC_Micrel.PcbLib',NULL,NULL,NULL,NULL,'Micrel','','PQFP-128','KSZ88955RQ','100Mbit/s','3.3V',NULL,'-40','+85','0.962','41.54C/W',1,'2013-10-02 13:11:50','KSZ8895RQI','\\\\FSERVER\\PCAD\\Altium\\Libs\\DSheet\\IC\\Micrel\\ksz8895mq_rq_fmq_ds.pdf',1,0,'KSZ8895RQI','KSZ8895MQI',0,NULL);
/*!40000 ALTER TABLE `ic_micrel` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_microcontroller`
--

DROP TABLE IF EXISTS `ic_microcontroller`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_microcontroller` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_microcontroller`
--

LOCK TABLES `ic_microcontroller` WRITE;
/*!40000 ALTER TABLE `ic_microcontroller` DISABLE KEYS */;
INSERT INTO `ic_microcontroller` VALUES (5,'STM32F407V','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Microcontroller.SchLib','LQFP-100','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Microcontroller.PcbLib','','','','','ST Microelectronics','STM32F407VET6','LQFP-100','STM32F407V','ARM ','3,3 В','','-40','85','0,465 Вт','',2008,'2014-02-28 14:57:13','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Microcontroller\\STM32F407VGT6_tds_eng.pdf',2,0,'STM32F407VET6','ST Microelectronics',0,'',1,'ARM',0,NULL,NULL);
/*!40000 ALTER TABLE `ic_microcontroller` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_opamp`
--

DROP TABLE IF EXISTS `ic_opamp`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_opamp` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=8 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_opamp`
--

LOCK TABLES `ic_opamp` WRITE;
/*!40000 ALTER TABLE `ic_opamp` DISABLE KEYS */;
INSERT INTO `ic_opamp` VALUES (5,'OPA4340UA','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_OpAmp.SchLib','SO-14','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_OpAmp.PcbLib','','','','','Texas Instruments','OPA4340UA','SO-14','OPA4340','94 дБ','5,5 В','','-40','+85','','',2001,'2014-07-28 15:20:01','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\OpAmp\\opa4340.pdf',3,0,'OPA4340UA','TI',0,'',1,'94',0,NULL,NULL),(6,'OPA340','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_OpAmp.SchLib','SOT23-5','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_OpAmp.PcbLib','','','','','Texas Instruments','OPA340NA/250','SOT23-5','OPA340','94 дБ','5,5 В','','-40','+85','','',2008,'2014-03-05 14:30:55','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\OpAmp\\opa4340.pdf',1,0,'Операционный усилитель OPA340NA/250','TI',0,'',1,'94',0,NULL,NULL),(7,'OPA4340UA','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_OpAmp.SchLib','SO-14','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_OpAmp.PcbLib','','','','','Analog Devices','AD8554AR','SO-14','AD8554','130 дБ','5,5 В','','-40','+85','','',2010,'2014-07-28 15:27:09','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\OpAmp\\AD8551_2_4_0.pdf',1,0,'AD8554AR','Analog Devices',0,'',1,'130',0,NULL,NULL);
/*!40000 ALTER TABLE `ic_opamp` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_opto`
--

DROP TABLE IF EXISTS `ic_opto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_opto` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=5 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_opto`
--

LOCK TABLES `ic_opto` WRITE;
/*!40000 ALTER TABLE `ic_opto` DISABLE KEYS */;
INSERT INTO `ic_opto` VALUES (1,'TLP187','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Opto.SchLib','11-4M1S','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Opto.PcbLib','','NULL','','','Toshiba Electronics','TLP187','11-4M1S','187','1000 %','1,25 В','','-55','110','0.2','-1.5 мВт/С',2001,'2014-04-08 15:49:38','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Opto\\TLP187_datasheet_en_20121107.pdf',9,0,'Оптрон TLP187','Toshiba',0,'',1,'1000',1,NULL,NULL);
/*!40000 ALTER TABLE `ic_opto` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_powermgmt`
--

DROP TABLE IF EXISTS `ic_powermgmt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_powermgmt` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_powermgmt`
--

LOCK TABLES `ic_powermgmt` WRITE;
/*!40000 ALTER TABLE `ic_powermgmt` DISABLE KEYS */;
INSERT INTO `ic_powermgmt` VALUES (1,'LTC4357','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_PowerMgmt.SchLib','MSOP8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_PowerMgmt.PcbLib','','','','','Linear Technology','LTC4357IMS8','MSOP8','LTCXD','80 В','80 В','','-40','85','0.245 Вт','',2001,'2014-02-28 14:51:46','','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Linear Technology\\Power\\4357fd.pdf',7,0,'LTC4357IMS8','LTC4357HMS8, LTC4357MPMS8',0,'',1,'80',14,NULL,NULL),(5,'BD9778HFP','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_PowerMgmt.SchLib','HRP7','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_PowerMgmt.PcbLib','','','','','Rohm','BD9778HFP','HRP7','BD9778','2 А','35 В','2%','-40','+125','1,98 Вт','',2001,'2014-02-28 14:51:57','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\PowerMgmt\\bd9778f-e.pdf',2,0,'DC-DC преобразователь BD9778HFP','Rohm',0,'',1,'2',18,NULL,NULL),(6,'LTC4415','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_PowerMgmt.SchLib','DFN16','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_PowerMgmt.PcbLib','','','','','Linear Technology','LTC4415IDHC','DFN16','LTC4415','250 мВ','6В','','-40','+125','','',2008,'2014-02-28 14:52:15','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\PowerMgmt\\4415fa.pdf',2,0,'LTC4415IDHC','Linear',0,'',1,'250',15,NULL,NULL),(7,'NCP1529','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_PowerMgmt.SchLib','TSOP-5','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_PowerMgmt.PcbLib','','','','','ON Semiconductor','NCP1529','TSOP-5','DXJ','1 А','7 В','3%','-40','85','','',2008,'2014-02-28 14:52:27','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\PowerMgmt\\ons~ncp1529.pdf',2,0,'NCP1529','On Semiconductor',0,'',1,'1',18,NULL,NULL),(8,'BD9781HFP','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_PowerMgmt.SchLib','HRP7','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_PowerMgmt.PcbLib','','','','','Rohm','BD9781HFP','HRP7','BD9781','4 А','35 В','2%','-40','+125','1,98 Вт','',2001,'2015-06-22 14:37:13','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\PowerMgmt\\bd9778f-e.pdf',1,0,'DC-DC преобразователь BD9781HFP','Rohm',0,'',1,'4',18,NULL,NULL);
/*!40000 ALTER TABLE `ic_powermgmt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_protection`
--

DROP TABLE IF EXISTS `ic_protection`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_protection` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_protection`
--

LOCK TABLES `ic_protection` WRITE;
/*!40000 ALTER TABLE `ic_protection` DISABLE KEYS */;
INSERT INTO `ic_protection` VALUES (5,'USBLC6','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Protection.SchLib','SOT23-6','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Protection.PcbLib','','','','','ST Microelectronics','USBLC6','SOT23-6','USBLC6','2 кан','6 В','','-40','+125','','',2008,'2014-02-28 14:53:10','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Protection\\st usblc6-2.pdf',2,0,'USBLC6','ST',0,'',1,'2',28,NULL,NULL),(6,'ITA6V5B3','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Protection.SchLib','SO-20','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Protection.PcbLib','','','','','ST Microelectronics','ITA6V5B3','SO-20','','8 кан','9,5 В','','-40','+85','','',2008,'2014-02-28 14:53:52','','',2,0,'ITA6V5B3','ST Microelectronics',0,'',1,'8',28,NULL,NULL),(7,'SP0305','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Protection.SchLib','SOT143-4','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Protection.PcbLib','','','','','Littelfuse','SP0503BAHTG','SOT143-4','','3 кан','5 В','','-40','+85','0,225 Вт','',2008,'2014-04-08 17:32:27','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Protection\\littelfuse_tvs_diode_array_spa_sp050_datasheet.pdf',3,0,'SP0503BAHTG','Littelfuse',0,'',1,'3',28,NULL,NULL),(8,'2tvs_cmn','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_Protection.SchLib','SOT23','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_Protection.PcbLib','','','','','Semtech','SM712','SOT-23','712','12 В','17 А','','-55','+125','400 Вт','',2001,'2015-06-10 15:18:35','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\Protection\\sm712.pdf',1,0,'Сборка защитных диодов SM712','Semtech',0,'',1,'12',14,NULL,NULL);
/*!40000 ALTER TABLE `ic_protection` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ic_vreference`
--

DROP TABLE IF EXISTS `ic_vreference`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ic_vreference` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ic_vreference`
--

LOCK TABLES `ic_vreference` WRITE;
/*!40000 ALTER TABLE `ic_vreference` DISABLE KEYS */;
INSERT INTO `ic_vreference` VALUES (5,'MAX6070','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\IC\\IC_VReference.SchLib','SOT23-6','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\IC\\IC_VReference.PcbLib','','','','','Maxim Integrated','MAX6070AAUT25+','SOT23-6','+ACPL','2,5 В','6 В','0,04%','-40','+125','348 мВт','',2008,'2014-02-28 14:52:42','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\IC\\VReference\\max6070.pdf',2,0,'MAX6070AAUT25+','Maxim',0,'',1,'2,5',14,NULL,NULL);
/*!40000 ALTER TABLE `ic_vreference` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `inductors`
--

DROP TABLE IF EXISTS `inductors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `inductors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `inductors`
--

LOCK TABLES `inductors` WRITE;
/*!40000 ALTER TABLE `inductors` DISABLE KEYS */;
INSERT INTO `inductors` VALUES (5,'INDUCTOR','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','SRR1208','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','Bourns Inc','SRR1208-101Y','SRR1208','101','100 мкГн','1,5 А','15%','-40','+125','','',2001,'2014-02-28 14:59:04','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\SRR1208.pdf',2,0,'Дроссель SRR1208 100 мкГн','bourns',0,'',1,'100',12,NULL,NULL),(6,'FM_INDUCTOR','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','Bourns Inc','MH1608-221Y','0603','-','220 Ом','2А','25%','-55','+125','','',2001,'2014-02-28 14:58:46','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\Bourns\\mh.pdf',2,0,'Ферр. ВЧ-дроссель MH1608-221Y','Bourns',0,'',1,'220',6,NULL,NULL),(7,'INDUCTOR','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','SDR0403','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','Bourns Inc','SDR0403-4R7ML','SDR0403','','4,7 мкГн','1,7 А','20%','-40','+125','','',2008,'2014-02-28 14:58:56','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\sdr0403.pdf',2,0,'Дроссель SDR0403 4,7 мкГн','Bourns',0,'',1,'4,7',12,NULL,NULL),(8,'PLY17','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','PLY17','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','Murata','PLY17BN1023R0A2','','','1 мГн','3 А','','-40','+85','','',2008,'2014-06-04 16:57:14','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\ply17.pdf',1,0,'Гибридный дроссель PLY17BN1023R0A2','Murata',0,'',0,'1',11,NULL,NULL),(9,'FM_INDUCTOR','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','EC36','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','НКП','Индуктивность EC36-150K','EC36','','15 мкГн','0,46 А','10%','-20','100','','',2008,'2014-06-05 13:36:12','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\ec.PDF',1,0,'Индуктивность EC36-150K','',0,'Индуктивность EC36-1',0,'15',12,NULL,NULL),(10,'INDUCTOR','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','SDR0403','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','Bourns Inc','SDR0403-6R8ML','SDR0403','','6,8 мкГн','1,41 А','20%','-40','+125','','',2010,'2014-07-16 12:04:42','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\sdr0403.pdf',1,0,'Дроссель SDR0403 6,8 мкГн','Bourns',0,'',1,'6,8',12,NULL,NULL),(11,'INDUCTOR','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','CDRH104R','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','Sumida','CDRH104RNP-150NC','CDRH104R','150','15 мкГн','3,4 А','30%','-40','105','','',2008,'2015-02-05 10:45:27','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\sumida~cdrh104r.pdf',1,0,'CDRH104RNP-150NC','Sumida',0,'',1,'15',12,NULL,NULL),(12,'FM_INDUCTOR','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','RCH-110','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','Sumida','RCH110NP-150M','','150M','15 мкГн','4,4 А','','-40','100','','',2008,'2015-02-05 15:11:58','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\RCH-110.pdf',1,0,'RCH110NP-150M','Sumida',0,'',0,'15',12,NULL,NULL),(13,'FM_INDUCTOR','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\inductors.SchLib','SDR0302','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\inductors.PcbLib','','','','','Bourns Inc','SDR0302-4R7ML','SDR0302','4R7','4,7 мкГн','1,35 А','20%','-40','+125','','',2001,'2015-06-17 16:39:32','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Inductors\\Bourns\\SDR0302.pdf',2,0,'Дроссель SDR0302 4,7 мкГн','Bourns',0,'',1,'4,7',12,NULL,NULL);
/*!40000 ALTER TABLE `inductors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mechanical`
--

DROP TABLE IF EXISTS `mechanical`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mechanical` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mechanical`
--

LOCK TABLES `mechanical` WRITE;
/*!40000 ALTER TABLE `mechanical` DISABLE KEYS */;
INSERT INTO `mechanical` VALUES (1,'5223979-1','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\mechanical.SchLib','5223979-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\mechanical.PcbLib','','','','','Tyco Electronics','5223979-1','5223979-1','',' ','250 В','','-55','125','','',2001,'2014-02-28 14:39:13','5223979-1','\\\\FSERVER\\PCAD\\Altium\\Libs\\DSheet\\Mechanical\\TE\\ENG_CD_5223979_O.pdf',3,0,'Гнездо направляющее 5223979-1','Tyco',0,'',1,'',0,NULL,NULL),(5,'223956-1','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\mechanical.SchLib','223956-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\mechanical.PcbLib','','','','','Tyco Electronics','223956-1','','',' ','','','-55','+125','','',2001,'2014-04-11 11:03:06','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Mechanical\\TE\\ENG_CD_223956_E2_baseFilename.pdf',1,0,'Штырь направляющий 223956-1','Tyco',0,'',0,'',0,NULL,NULL);
/*!40000 ALTER TABLE `mechanical` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `modules`
--

DROP TABLE IF EXISTS `modules`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `modules` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(10) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `modules`
--

LOCK TABLES `modules` WRITE;
/*!40000 ALTER TABLE `modules` DISABLE KEYS */;
INSERT INTO `modules` VALUES (5,'VDX-6357D','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\modules.SchLib','VDX-6357D','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\modules.PcbLib','','','','','ICOP','VDX-6357RD-512','PC-104','VDX-6357','VRX ','800 МГц','','-20','+70','4 Вт','',2001,'2014-05-28 14:57:31','512 Мб','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Modules\\VDX-6357D_UM_v1r0A.pdf',1,0,'Модуль процессорный VDX-6357RD-512','PC-104',0,'',0,'VRX',0,NULL,NULL);
/*!40000 ALTER TABLE `modules` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `opto`
--

DROP TABLE IF EXISTS `opto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `opto` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=14 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `opto`
--

LOCK TABLES `opto` WRITE;
/*!40000 ALTER TABLE `opto` DISABLE KEYS */;
INSERT INTO `opto` VALUES (1,'LED','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','L-1384AD_1ID','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Kingbright Electronic Co, Ltd','L-1384AD/1ID','L-1384AD','','красн цвет','2.5 В','','-40','+85','0.075 Вт','-0.42 мА/С',2001,'2014-03-03 13:10:30','L-1384AD','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Kingbright\\l-1384ad-1id.pdf',8,0,'Светодиод L-1384AD/1ID','Kingbright',0,'',0,'красн',34,NULL,NULL),(5,'LED-20','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','DC-20_20','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Kingbright Electronic Co, Ltd','DC-20/20YWA','DC-20/20','','20 шт','2.5 В','','-40','+85','75 мВт','',2001,'2014-03-04 10:35:14','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Kingbright\\DC-20-20YWA.pdf',6,0,'Светодиодная линейка DC-20/20YWA','Kingbright',0,'',0,'20',26,NULL,NULL),(6,'LED-4','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','L-914CK-4GDT','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Kingbright Electronic Co, Ltd','L-914CK/4GDT','L-914CK/4','','4 шт','2,5 В','','-40','+85','62,5 мВт','',2008,'2014-02-28 14:39:37','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Kingbright\\l-914ck-4gdt.pdf',2,0,'Сборка из 4 светодиодов L-914CK/4GDT','Kingbright',0,'',0,'4',26,NULL,NULL),(7,'LED-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','L-7104MD/2GD','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Kingbright Electronic Co, Ltd','L-7104MD/GD','L-7104MD','','зеленый цв','2,5 В','','-40','+85','0,062 Вт','',2008,'2014-04-15 18:11:50','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Kingbright\\l-7104md-2gd.pdf',1,0,'Светодиод L-7104MD/GD','Kingbright',0,'',0,'зеленый',34,NULL,NULL),(8,'LED','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','KP-1608','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Kingbright Electronic Co, Ltd','KP-1608ID','0603','','красн цвет','2,5 В','','-40','+85','0,105 Вт','',2008,'2014-05-26 12:37:41','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Kingbright\\KP-1608ID.pdf',1,0,'Светодиод KP-1608ID','Kingbright',0,'',1,'красн',34,NULL,NULL),(9,'Photocoupler','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','TLP-182','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Toshiba Electronics','TLP182','11-4M1S','','50 %','1,4 В','','-55','125','0,2 Вт','',2008,'2014-05-26 12:54:56','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Toshiba\\TLP182_datasheet_en_20130913.pdf',1,0,'Опттрон TLP182','Toshiba',0,'',1,'50',1,NULL,NULL),(10,'Photocoupler+Schmitt','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','DIP-6SM','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Fairchild Semiconductor','H11L1SM','DIP-6SM','','1,6 мА','1,5 В','','-40','+85','','',2008,'2014-06-24 14:39:32','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Fairchild\\H11L1M.pdf',1,0,'H11L1SM','Оптрон с выходом триггер Шмитта H11L1SM',0,'',1,'1,6',19,NULL,NULL),(11,'LED','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','KPA-3010G','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Kingbright Electronic Co, Ltd','KPA-3010SGC','KPA3010','','зел цвет','2,5 В','','-40','+85','0,0625 Вт','',2001,'2015-06-25 11:40:51','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Kingbright\\KPA-3010SGC(Ver.24B).pdf',4,0,'Светодиод угловой KPA-3010SGC','Kingbright',0,'',1,'зел',34,NULL,NULL),(12,'LED','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','KPA-3010Y','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Kingbright Electronic Co, Ltd','KPA-3010SYC','KPA3010','','желт цвет','2,5 В','','-40','+85','0,0625 Вт','',2001,'2015-06-25 11:53:19','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Kingbright\\KPA-3010SYCK(Ver.15B).pdf',1,0,'Светодиод угловой KPA-3010SYC','Kingbright',0,'',1,'желт',34,NULL,NULL),(13,'LED-2','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\opto.SchLib','L-4060VH-2','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\opto.PcbLib','','','','','Kingbright Electronic Co, Ltd','L-4060VH/2ID','','','красный цв','2,5 В','','','','0,075 Вт','',2008,'2015-09-17 14:27:24','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Opto\\Kingbright\\L-4060VH-2ID HightEfficiency Red Bi-Level 1,8mm.pdf',1,0,'2 красных светодиода в сборке L-4060VH/2ID','Kingbright',0,'',0,'красный',34,NULL,NULL);
/*!40000 ALTER TABLE `opto` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `oscillators`
--

DROP TABLE IF EXISTS `oscillators`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `oscillators` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `oscillators`
--

LOCK TABLES `oscillators` WRITE;
/*!40000 ALTER TABLE `oscillators` DISABLE KEYS */;
INSERT INTO `oscillators` VALUES (5,'QuartzOscillator','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\Oscillators.SchLib','HC-49_US','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\Oscillators.PcbLib','','','','','Geyer Electronic','KX-3HT 25.0MHz','HC-49US','','25.0 МГц','16 пФ','30 pp','-40','+85','','',2008,'2014-03-03 13:06:54','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Oscillators\\geyer~kx-3h.pdf',6,0,'KX-3HT 25.0MHz','Geyer',0,'',0,'25.0',0,NULL,NULL),(6,'QuartzOscillator','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\Oscillators.SchLib','KX-38','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\Oscillators.PcbLib','','','','','Geyer Electronic','KX-38T','KX-38','','32,768 кГц','6 пФ','20 pp','-40','+85','','',2008,'2014-02-28 15:06:57','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Oscillators\\GEYER-KX-38_01.pdf',2,0,'Кварц KX-38T 32,768 кГц 6пФ','Geyer',0,'',0,'32,768',0,NULL,NULL);
/*!40000 ALTER TABLE `oscillators` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `posistors`
--

DROP TABLE IF EXISTS `posistors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `posistors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(10) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `posistors`
--

LOCK TABLES `posistors` WRITE;
/*!40000 ALTER TABLE `posistors` DISABLE KEYS */;
INSERT INTO `posistors` VALUES (5,'Posistor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\Posistors.SchLib','4532','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\Posistors.PcbLib','','','','','Bourns Inc','MF-MSMF014','4532','14xx','140 мА','6,5 Ом','','-40','+85','0,8 Вт','',2008,'2014-05-20 12:51:22','0,15 с','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Posistors\\Bourns\\mf-msmf.pdf',1,0,'Позистор MF-MSMF014','Bourns',0,'',1,'140',19,NULL,NULL),(6,'TBU','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\Posistors.SchLib','TBU-CA','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\Posistors.PcbLib','','','','','Bourns Inc','TBU-CA085-200-WH','TBU-CA','A8520','850 В ','425 В','','-55','+125','3 Вт','40 C/W',2001,'2015-06-17 12:43:16','300 мА','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Posistors\\Bourns\\TBU-CA.pdf',1,0,'Позистор TBU-CA085-200','Bourns',0,'',1,'850 В',0,NULL,NULL);
/*!40000 ALTER TABLE `posistors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `power`
--

DROP TABLE IF EXISTS `power`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `power` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `power`
--

LOCK TABLES `power` WRITE;
/*!40000 ALTER TABLE `power` DISABLE KEYS */;
INSERT INTO `power` VALUES (1,'EPP-150','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\power.SchLib','EPP-150','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\power.PcbLib','','','','','Meanwell','EPP-150','EPP-150','','150 Вт','24 В','','-30','+70','150 Вт','0,03 С/Вт',2001,'2014-02-28 14:40:41','EPP-150','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Power\\Meanwell\\EPP-150-spec.pdf',6,0,'Источник питания EPP-150 (CUS100M-24)','Meanwell (TDK Lambda)',0,'',0,'150',22,NULL,NULL),(2,'SKE15','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\power.SchLib','SKE15','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\power.PcbLib','','','','','Meanwell','SKE15B-24','SKE15','','15 Вт','24 В','','-25','+71','','',2008,'2014-03-04 13:59:16','SKE15B-24','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Power\\Meanwell\\ske15-spec.pdf',1,0,'Источник питания SKE15B-24','Meanwell',0,'',0,'15',22,NULL,NULL),(3,'MEF1S2405SP3C','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\power.SchLib','SIP7','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\power.PcbLib','','','','','Murata','MEF1S2405SP3C','SIP7','','1 Вт','','','-40','85','','',2010,'2014-07-16 11:31:52','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Power\\MURATA\\kdc_mef1-272780.pdf',2,1,'Преобразователь напряжения изолирующий MEF1S2405SP3C ','Аналоги: TMV-2405S, TRV1-2411, PUS-2405-3K, F',0,'',0,'1',22,NULL,NULL),(4,'smd 12pin','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\power.SchLib','SMD 12pin','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\power.PcbLib','','','','','Aimtec','AM1L-0505SH30-NZ','SMD12','AM1L-0505S','1 Вт','3000 В','','-40','+85','','',2001,'2015-06-17 15:44:29','70 %','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Power\\Aimtec\\am1l-n-z.pdf',1,0,'DC-DC преобразователь AM1L-0505SH30-NZ','Aimtec',0,'',1,'1',22,NULL,NULL),(5,'AM1D  -  0303DH30Z','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\power.SchLib','SIP7D','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\power.PcbLib','','','','','Aimtec','AM1D-0303DH30Z','SIP7D','','1 Вт','Uin=3,3V','','-40','85','','',2010,'2016-02-04 12:53:14','Uout=+/-3,3V','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Power\\Aimtec\\AM1D-Z.pdf',1,1,'Преобразователь напряжения изолирующий биполярный AM1D-0303DH30Z','',0,'',0,'1',22,NULL,NULL);
/*!40000 ALTER TABLE `power` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `qaltium`
--

DROP TABLE IF EXISTS `qaltium`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `qaltium` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `tablename` varchar(30) NOT NULL,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `date` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `qaltium`
--

LOCK TABLES `qaltium` WRITE;
/*!40000 ALTER TABLE `qaltium` DISABLE KEYS */;
/*!40000 ALTER TABLE `qaltium` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `relay`
--

DROP TABLE IF EXISTS `relay`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `relay` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(10) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(45) DEFAULT NULL,
  `par5` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `relay`
--

LOCK TABLES `relay` WRITE;
/*!40000 ALTER TABLE `relay` DISABLE KEYS */;
INSERT INTO `relay` VALUES (5,'1C','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\relay.SchLib','TE PB114024','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\relay.PcbLib',NULL,NULL,NULL,NULL,'Tyco Electronics','9-1415029-1',NULL,NULL,'24 В','SPDT',NULL,'-40','+85',NULL,NULL,2008,'2015-09-15 14:30:21','0,6A @ 220','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Relay\\TE 9-1415029-1 .pdf',1,0,'9-1415029-1','TE',0,NULL,0,'24',14,NULL,NULL);
/*!40000 ALTER TABLE `relay` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `resistors`
--

DROP TABLE IF EXISTS `resistors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `resistors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=73 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `resistors`
--

LOCK TABLES `resistors` WRITE;
/*!40000 ALTER TABLE `resistors` DISABLE KEYS */;
INSERT INTO `resistors` VALUES (1,'Resistor 0.5W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','2010 (5025)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,5 Вт-180 кОм-5%,2010','2010','184','180 кОм','200 В','5%','-55','155','0,5 Вт','200 ppm/C',2001,'2014-02-28 14:47:42','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC2010.pdf',4,0,'Чип Рез. 0,5 Вт-180 кОм-5%,тип 2010','',0,'Чип',1,'180',7,NULL,NULL),(2,'Resistor 0.5W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','2010 (5025)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,5 Вт-2.4 кОм-5%,2010','2010','242','2.4 кОм','200 В','5%','-55','155','0,5 Вт','200 ppm/C',2001,'2014-02-28 14:47:31','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC2010.pdf',4,0,'Чип Рез. 0,5 Вт-2.4 кОм-5%,тип 2010','',0,'Чип',1,'2.4',7,NULL,NULL),(3,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип рез. 0,063-100 Ом-5%,0603 (1608)','0603 (1608)','101','100 Ом','50 В','5%','-55','125','0,063 Вт','100 ppm/C',2001,'2013-11-05 13:17:40','Чип рез.','FSERVERPCADAltiumLibsDSheetResistorsYageoPYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип рез. 0,063-100 Ом-5%,0603 (1608)','',1,NULL,NULL,NULL,NULL,NULL,NULL),(6,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-10 кОм-тип 0603','0603','103','10 кОм','50 В','5%','-55','125','0,063 Вт','100 ppm/C',2001,'2014-04-04 09:58:06','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',6,0,'Чип Рез. 0,063 Вт-10 кОм-тип 0603','',0,'Чип',1,'10',7,NULL,NULL),(9,'Res 8R0402 5-10','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','Res 8R0402 5-10','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','Yageo','YC158TJR-0710K(L)','8R0402','103','10 кОм','25 В','5%','-55','+155','0,125 Вт','200 ppm/C',2001,'2014-02-28 14:47:03','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-YC158_358_5_RoHS_L_3.pdf',8,0,'Рез. матрица YC158TJR-0710K(L)','Yageo, Bourns',0,'',1,'10',7,NULL,NULL),(10,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','Yageo','RC0603FR-0712K4(L)','0603','1242','12,4 кОм','75 В','1%','-55','+155','0,1 Вт','100 ppm/C',2001,'2014-02-28 14:46:48','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',3,0,'Резистор 0,063Вт-12,4 кОм-1%, 0603','Yageo, Bourns',0,'',1,'12,4',7,NULL,NULL),(11,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-12,4 кОм-1%,тип 0603','0603','1242','12,4 кОм','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2001,'2014-02-28 14:46:34','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',3,0,'Чип Рез. 0,063 Вт-12,4 кОм-1%,тип 0603','',0,'Чип',1,'12,4',7,NULL,NULL),(12,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-3,3 кОм-5%,0603','0603','332','3,3 кОм','50 В','5%','-55','125','0,063 Вт','100 ppm/C',2001,'2014-02-28 14:46:23','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',4,0,'Чип Рез. 0,063 Вт-3,3 кОм-5%,тип 0603','',0,'Чип',1,'3,3',7,NULL,NULL),(13,'Res 4R0603','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','Res 4R0603','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','Yageo','YC164-JR-07220R','4x0603','220R','220 Ом','50 В','5%','-55','+155','0,063 Вт','200 ppm/C',2001,'2014-02-28 14:46:15','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\YC164.pdf',3,0,'Резисторная сборка YC164 220 Ом, 5%','Yageo',0,'',1,'220',6,NULL,NULL),(14,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-4,7 кОм-тип 0603','0603','472','4,7 кОм','50 В','5%','-55','125','0,063 Вт','100 ppm/C',2001,'2014-02-28 14:46:04','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',3,0,'Чип Рез. 0,063 Вт-4,7 кОм-5%,тип 0603','',0,'Чип',1,'4,7',7,NULL,NULL),(15,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-100 кОм-5%,тип 0603','0603','104','100 кОм','50 В','5%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-02-28 14:45:50','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',3,0,'Чип Рез. 0,063 Вт-100 кОм-5%,тип 0603','',0,'Чип',1,'100',7,NULL,NULL),(16,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-22 кОм-5%,0603','0603','223','22 кОм','50 В','5%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-02-28 14:45:20','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,063 Вт-22 кОм-5%,тип 0603','',0,'Чип',1,'22',7,NULL,NULL),(17,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-750 Ом-5%,0603','0603','751','750 Ом','50 В','5%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-02-28 14:45:07','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,063 Вт-750 Ом-5%,тип 0603','',0,'Чип',1,'750',6,NULL,NULL),(18,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','1206 (3216)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','КБ Икар','Р1-16М-0,125(1206) А -49,9 кОм+0,5%-0,5 Г','1206','5002','49,9 кОм','100 В','0,5%','-60','+155','0,125 Вт','50 ppm',2008,'2014-03-03 13:41:56','','',4,0,'Р1-16М-0,125(1206) А -49,9 кОм+05%-0,5 Г','КБ Икар',0,'',1,'50',7,NULL,NULL),(19,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','1206 (3216)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','КБ Икар','Р1-16М-0,125(1206) А -10 кОм+0,5%-0,5 Г','1206','1002','10 кОм','100 В','0,5%','-60','125','0,125 Вт','50 ppm',2008,'2014-04-04 09:58:29','','',4,0,'Р1-16М-0,125(1206) А -10кОм+0,5%-0,5 Г','КБ Икар',0,'',1,'10',7,NULL,NULL),(20,'Resistor 1W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','2512 (6332)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','КБ Икар','Р1-8МП-1,0(2512) А -249 Ом+0,5%-0,5 Л','2512','250R','250 Ом','100 В','0,5%','-60','155','1 Вт','150 ppm',2008,'2014-04-25 11:49:25','','',5,0,'Р1-8МП-1,0(2512) А -249 Ом+0,5%-0,5 Л','КБ Икар',0,'',1,'250',6,NULL,NULL),(21,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-390 кОм-5%,тип 0603','0603','394','390 кОм','50 В','5%','-55','+155','0,1 Вт','100 ppm/C',2001,'2014-02-28 14:44:20','','',2,0,'Чип Рез. 0,1 Вт-390 кОм-5%,тип 0603','',0,'Чип',1,'390',7,NULL,NULL),(22,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-150 кОм-5%,тип 0603','0603','154','150 кОм','50 В','5%','-55','+155','0,1 Вт','200 ppm/C',2001,'2014-02-28 14:44:09','','',3,0,'Чип Рез. 0,1 Вт-150 кОм-5%,тип 0603','',0,'Чип',1,'150',7,NULL,NULL),(23,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-12 кОм-1%,тип 0603','0603','123','12 кОм','50 В','1%','-55','+155','0,1 Вт','200 ppm/C',2001,'2014-02-28 14:44:00','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,1 Вт-12 кОм-1%,тип 0603','RC0603FR-0712K',0,'Чип',1,'12',7,NULL,NULL),(24,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-27 кОм-1%,тип 0603','0603','273','27 кОм','50 В','1%','-55','+155','0,1 Вт','200 ppm/C',2001,'2014-02-28 14:43:46','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,1 Вт-27 кОм-1%,тип 0603','',0,'Чип',1,'27',7,NULL,NULL),(25,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,125 Вт-22 Ом-5%,0805','0805','22R','22 Ом','100 В','5%','-55','125','0,125 Вт','100 ppm/C',2008,'2014-02-28 14:43:34','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,125 Вт-22 Ом-5%,тип 0805','',0,'Чип',1,'22',6,NULL,NULL),(26,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,125 Вт-4,87 кОм-1%,тип 0805','0805','4871','4,87 кОм','100 В','1%','-40','+125','0,125 Вт','100 ppm/C',2001,'2014-02-28 14:43:01','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',3,0,'Чип Рез. 0,125 Вт-4,87 кОм-1%,тип 0805','',0,'Чип',1,'4,87',7,NULL,NULL),(27,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','КБ Икар','Р1-16М-0,062(0805) А -20кОм+0,5%-0,5 Д','0805','2002','20 кОм','100 В','0,5%','-60','+155','0,062','50 ppm',2008,'2014-02-28 14:43:17','','',3,0,'Р1-16М-0,062(0805) А -20кОм+0,5%-0,5 Д','КБ Икар',0,'',1,'20',7,NULL,NULL),(28,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-0 Ом-тип 0603','0603','0','0 Ом','2 А','','-55','125','0,063 Вт','',2001,'2014-02-28 14:42:22','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Vishay\\dcrcw.pdf',3,0,'Чип Рез. 0,063 Вт-0 Ом-,тип 0603','Перемычка',0,'Чип',1,'0',6,NULL,NULL),(29,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,125 Вт-6,49 кОм-1%,0805','0805','6491','6,49 кОм','50 В','1%','-40','+85','0,125 Вт','100 ppm',2008,'2014-02-28 14:42:10','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,125 Вт-6,49 кОм-1%,тип 0805','',0,'Чип',1,'6,49',7,NULL,NULL),(31,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-49,9 Ом-1%,тип 0603','0603','49R9','49,9 Ом','50 В','1%','-55','+155','0,1 Вт','200 ppm/C',2001,'2014-02-28 14:42:00','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,1 Вт-49,9 Ом-1%,тип 0603','RC0603FR-0749R9L',0,'Чип',1,'49,9',6,NULL,NULL),(32,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-1,2 МОм-1%,тип 0603','0603','125','1,2 МОм','50 В','1%','-55','+155','0,1 Вт','200 ppm/C',2008,'2014-02-28 14:41:49','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',3,0,'Чип Рез. 0,1 Вт-1,2 МОм-1%,тип 0603','RC0603FR-071M2L',0,'Чип',1,'1,2',5,NULL,NULL),(33,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-270 кОм-1%,тип 0603','0603','274','270 кОм','50 В','1%','-40','+155','0,1 Вт','200 ppm/C',2008,'2014-02-28 14:41:35','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,1 Вт-270 кОм-1%,тип 0603','RC0603FR-07270KL',0,'Чип',1,'270',7,NULL,NULL),(34,'Resistor 0.25W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','1206 (3216)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,25 Вт-51 Ом-5%,тип 1206','1206','51R','51 Ом','100 В','5%','-40','+125','0,25 Вт','200 ppm/C',2001,'2014-02-28 14:41:24','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\General Type_MFR.pdf',2,0,'Чип Рез. 0,25 Вт-51 Ом-5%,тип 1206','',0,'Чип',1,'51',6,NULL,NULL),(35,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-1 кОм-5%,тип 0603','0603','102','1 кОм','50 В','5%','-55','+155','0,1 Вт','200 ppm/C',2001,'2014-02-28 14:41:08','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,1 Вт-1 кОм-5%,тип 0603','',0,'Чип',1,'1',7,NULL,NULL),(36,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,125 Вт-180 Ом-тип 0603','0603','181','180 Ом','25 В','5%','-40','+85','0,125 Вт','200 ppm',2008,'2014-02-28 14:40:54','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,125 Вт-180 Ом-5%,тип 0603','RC0603JR-07180RL',0,'Чип',1,'180',6,NULL,NULL),(37,'Resistor 1W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','2512 (6332)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 1 Вт-270 Ом-5%,тип 2512','2512','271','270 Ом','16 В','5%','-55','+155','1 Вт','200 ppm',2008,'2014-03-03 14:53:52',NULL,'\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC2512.pdf',1,1,'Чип Рез. 1 Вт-270 Ом-5%,тип 2512','RC2512JK-07270RL',0,'Чип',NULL,NULL,NULL,NULL,NULL),(38,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,125 Вт-10 кОм-0,5%,тип 0805','0805','1002','10 кОм','35 В','0,5%','-55','+155','0,125 Вт','50 ppm/C',2008,'2014-05-14 17:33:36','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RT.pdf',1,0,'Чип Рез. 0,125 Вт-10 кОм-0,5%,тип 0805','Yageo RT0805DRE0710R0L',0,'Чип',1,'10',7,NULL,NULL),(39,'Resistor 0.5W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','1210 (3225)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,5 Вт-100 Ом-тип 1210','1210','101','100 Ом','200 В','5%','-55','155','0,5 Вт','100 ppm/C',2008,'2014-05-19 14:12:41','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC1210.pdf',1,0,'Чип Рез. 0,5 Вт-100 Ом-тип 1210','RC1210JR-07100RL',0,'Чип',1,'100',6,NULL,NULL),(40,'Resistor 0.5W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','2010 (5025)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,5 Вт-18 кОм-тип 2010','2010','183','18 кОм','200 В','5%','-55','155','0,5 Вт','100 ppm',2008,'2014-05-23 16:35:11','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC1210.pdf',1,0,'Чип Рез. 0,5 Вт-18 кОм-тип 2010','RC2010JK-0718KL',0,'Чип',1,'18',7,NULL,NULL),(41,'Resistor 0.25W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','1206 (3216)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,25 Вт-180 кОм-5%,тип 1206','1206','184','180 кОм','200 В','5%','-55','155','0,25 Вт','100 ppm/C',2008,'2014-05-23 16:46:08','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC1206.pdf',1,0,'Чип Рез. 0,25 Вт-180 кОм-5%,тип 1206','',0,'Чип',1,'180',7,NULL,NULL),(42,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,125 Вт-6,8 кОм-тип 0805','0805','682','6,8 кОм','150 В','5%','-55','155','0,125 Вт','100 ppm/C',2008,'2014-05-23 18:24:17','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC0805.pdf',1,0,'Чип Рез. 0,125 Вт-6,8 кОм-тип 0805','',0,'Чип',1,'6,8',7,NULL,NULL),(43,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-220 Ом-тип 0603','0603','221','220 Ом','75 В','5%','-55','155','0,063 Вт','100 ppm/C',2008,'2014-05-30 10:39:43','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,063 Вт-220 Ом-тип 0603','RC0603JR-07220RL',0,'Чип',1,'220',6,NULL,NULL),(44,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. -6,49 кОм-1%,тип 0603','0603','79С','6,49 кОм','50 В','1%','-55','125','','',2008,'2014-07-09 18:48:40','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. -6,49 кОм-1%,тип 0603','',0,'Чип',1,'6,49',7,NULL,NULL),(45,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,125','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,125-110кОм-0,1%-1.0-А','','','249 Ом','','0,1%','-60','+155 ','0,125 Вт','25ppm/гр',2010,'2014-07-28 15:01:44','','',3,1,'Резистор С2-29В-0,125-110кОм-0,1%-1.0-А','ОЖ0.467.099 ТУ',0,'',0,'249',6,NULL,NULL),(46,'Resistor 0.25W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,25','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,25-110кОм-0,1%-1,0-А','','','110 кОм','','0,1%','-60','+155','0,25 Вт','+/-25 ppm/',2010,'2014-07-28 15:01:15','','',2,0,'Резистор С2-29В-0,25-110кОм-0,1%-1,0-А','ОЖ0.467.099 ТУ',0,'',0,'110',7,NULL,NULL),(47,'Resistor 0.25W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,25','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,25-1,0 МОм-0,1%-1,0-А','','','1,0 МОм','','0,1%','-60','+155','0,25 Вт','+/-25 ppm/',2010,'2014-07-28 15:00:28','','',1,0,'Резистор С2-29В-0,25-1,0 МОм-0,1%-1,0-А','ОЖ0.467.099 ТУ',0,'',0,'1,0',5,NULL,NULL),(48,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,125','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,125-9,53 кОм-0,1%-1.0-А','','','9,53 кОм','','0,1%','-60','+155 ','0,125 Вт','25ppm/гр',2010,'2014-07-28 15:02:53','','',1,1,'Резистор С2-29В-0,125-9,53 кОм-0,1%-1.0-А','ОЖ0.467.099 ТУ',0,'',0,'9,53',7,NULL,NULL),(49,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,125','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,125-274 Ом-0,1%-1.0-А','','','274 Ом','','0,1%','-60','+155 ','0,125 Вт','25ppm/гр',2010,'2014-07-28 16:31:54','','',1,1,'Резистор С2-29В-0,125-274 Ом-0,1%-1.0-А','ОЖ0.467.099 ТУ',0,'',0,'274',6,NULL,NULL),(50,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,125','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,125-249 Ом-0,1%-1.0-А','','','249 Ом','','0,1%','-60','+155 ','0,125 Вт','25ppm/гр',2010,'2014-07-28 17:08:03','','',1,1,'Резистор С2-29В-0,125-249 Ом-0,1%-1.0-А','ОЖ0.467.099 ТУ',0,'',0,'249',6,NULL,NULL),(51,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-47 кОм-тип 0603','0603','154','47 кОм','50 В','5%','-55','+155','0,1 Вт','200 ppm/C',2010,'2014-08-12 14:26:23','','',1,0,'Чип Рез. 0,1 Вт-47 кОм-тип 0603','',0,'Чип',1,'47',7,NULL,NULL),(52,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-3,9 кОм-тип 0603','0603','154','3,9 кОм','50 В','5%','-55','+155','0,1 Вт','200 ppm/C',2010,'2014-08-12 14:28:35','','',3,0,'Чип Рез. 0,1 Вт-3,9 кОм-тип 0603','',0,'Чип',1,'3,9',7,NULL,NULL),(53,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-10 кОм-тип 0603','0603','154','10 кОм','50 В','5%','-55','+155','0,1 Вт','200 ppm/C',2010,'2014-08-12 14:28:13','','',1,0,'Чип Рез. 0,1 Вт-10 кОм-тип 0603','',0,'Чип',1,'10',7,NULL,NULL),(54,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,125 Вт-1,5 МОм-тип 0805','0805','682','1,5 МОм','150 В','5%','-55','155','0,125 Вт','100 ppm/C',2010,'2014-08-12 14:47:16','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC0805.pdf',1,0,'Чип Рез. 0,125 Вт-1,5 МОм-тип 0805','',0,'Чип',1,'1,5',5,NULL,NULL),(55,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-13 кОм-1%,тип 0603','0603','12C','13 кОм','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-08-13 11:20:23','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',3,0,'Чип Рез. 0,063 Вт-13 кОм-1%,тип 0603','',0,'Чип',1,'13',7,NULL,NULL),(56,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-15,4 кОм-1%,тип 0603','0603','19C','15,4 кОм','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-08-13 11:20:12','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,063 Вт-15,4 кОм-1%,тип 0603','',0,'Чип',1,'15,4',7,NULL,NULL),(57,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-17,8 кОм-1%,тип 0603','0603','25C','17,8 кОм','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-08-13 11:20:00','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,063 Вт-17,8 кОм-1%,тип 0603','',0,'Чип',1,'17,8',7,NULL,NULL),(58,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-16,2 кОм-1%,тип 0603','0603','21C','16,2 кОм','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-08-13 11:19:23','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,063 Вт-16,2 кОм-1%,тип 0603','',0,'Чип',1,'16,2',7,NULL,NULL),(59,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-18,7 кОм-1%,тип 0603','0603','27C','18,7 кОм','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-08-13 11:19:03','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',2,0,'Чип Рез. 0,063 Вт-18,7 кОм-1%,тип 0603','',0,'Чип',1,'18,7',7,NULL,NULL),(60,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-60,4 Ом-1%,тип 0603','0603','76R','60,4 Ом','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-08-13 11:15:10','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,063 Вт-60,4 Ом-1%,тип 0603','',0,'Чип',1,'60,4',6,NULL,NULL),(61,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-80,6 Ом-1%,тип 0603','0603','87R','80,6 Ом','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-08-13 11:17:06','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,063 Вт-80,6 Ом-1%,тип 0603','',0,'Чип',1,'80,6',6,NULL,NULL),(62,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-162 Ом-1%,тип 0603','0603','21A','162 Ом','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2008,'2014-08-13 11:18:12','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,063 Вт-162 Ом-1%,тип 0603','',0,'Чип',1,'162',6,NULL,NULL),(63,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-47,5 кОм-1%,тип 0603','0603','4753','47,5 кОм','50 В','1%','-55','+155','0,1 Вт','200 ppm/C',2001,'2014-09-24 13:03:41','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,1 Вт-47,5 кОм-1%,тип 0603','',0,'Чип',1,'47,5',7,NULL,NULL),(64,'Resistor 0.25W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,25','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,25-165Ом -0,1%-1,0-А','С2-29','','165 Ом','350В','0,1%','-60','155','0,25 Вт','25 ppm',2008,'2014-12-03 11:35:08','','',1,0,'С2-29В-0,25-165Ом -0,1%-1,0-А','ОЖ0.467.130 ТУ',0,'',0,'165',6,NULL,NULL),(65,'Resistor 0.25W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,25','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,25-60,4 Ом -0,1%-1,0-А','С2-29','','60,4 Ом','350В','0,1%','-60','155','0,25 Вт','25 ppm',2008,'2014-12-03 11:36:21','','',1,0,'С2-29В-0,25-60,4 Ом -0,1%-1,0-А','ОЖ0.467.130 ТУ',0,'',0,'60,4',6,NULL,NULL),(66,'Resistor 0.25W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','С2-29В-0,25','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','ОАО Эркон','С2-29В-0,25-12,4 кОм -0,1%-1,0-А','С2-29','','12,4 кОм','350В','0,1%','-60','155','0,25 Вт','25 ppm',2008,'2014-12-03 11:37:13','','',1,0,'С2-29В-0,25-12,4 кОм -0,1%-1,0-А','ОЖ0.467.130 ТУ',0,'',0,'12,4',7,NULL,NULL),(67,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-560 Ом-5 %,тип 0603','0603','561','560 Ом','50 В','5 %','-55','+155','0,1 Вт','100 ppm/C',2001,'2015-06-16 17:23:43','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,1 Вт-560 Ом-5 %,тип 0603','',0,'Чип',1,'560',6,NULL,NULL),(68,'Resistor 0.5W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','1210 (3225)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,5 Вт-120 Ом-5 %,тип 1210','1210','121','120 Ом','200 В','5 %','-55','+155','0,5 Вт','100 ppm/C',2001,'2015-06-16 17:28:49','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC1210.pdf',1,0,'Чип Рез. 0,5 Вт-120 Ом-5 %,тип 1210','',0,'Чип',1,'120',6,NULL,NULL),(69,'Resistor 0.125W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0805 (2012)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,125 Вт-240 Ом-тип 0805','0805','241','240 Ом','150 В','5%','-55','155','0,125 Вт','100 ppm/C',2001,'2015-06-17 16:01:02','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\RC0805.pdf',1,0,'Чип Рез. 0,125 Вт-240 Ом-тип 0805','',0,'Чип',1,'240',6,NULL,NULL),(70,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,1 Вт-390 Ом-тип 0603','0603','391','390 Ом','50 В','5%','-55','+155','0,1 Вт','100 ppm/C',2001,'2015-06-24 13:01:13','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,1 Вт-390 Ом-тип 0603','',0,'Чип',1,'390',6,NULL,NULL),(71,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-56 Ом-5%,тип 0603','0603','73R','56 Ом','75 В','5%','-40','105','0,063 Вт','100 ppm',2008,'2015-09-24 14:33:36','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,063 Вт-56 Ом-5%,тип 0603','RC0603FR-0756RL',0,'Чип',1,'56',6,NULL,NULL),(72,'Resistor 0.063W','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\resistors.SchLib','0603 (1608)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\resistors.PcbLib','','','','','НКП','Чип Рез. 0,063 Вт-1,5 кОм-тип 0603','0603','12C','1,5 кОм','50 В','1%','-55','125','0,063 Вт','100 ppm/C',2010,'2016-01-26 15:08:17','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Resistors\\Yageo\\PYu-RC0603_51_RoHS_L_6.pdf',1,0,'Чип Рез. 0,063 Вт-1,5 кОм-тип 0603','',0,'Чип',1,'1,5',7,NULL,NULL);
/*!40000 ALTER TABLE `resistors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `switches`
--

DROP TABLE IF EXISTS `switches`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `switches` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=17 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `switches`
--

LOCK TABLES `switches` WRITE;
/*!40000 ALTER TABLE `switches` DISABLE KEYS */;
INSERT INTO `switches` VALUES (7,'1-1571983-0','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','1-1571983-0','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','Tyco Electronics','1-1571983-0','GDH08S04','ALC','8 поз','24 В','','-30','+85','','',2001,'2014-02-28 14:49:30','1-1571983-0','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\ENG_CD_1571983_C3.pdf',2,0,'DIP-переключатель 1-1571983-0','Tyco',0,'',1,'8',27,NULL,NULL),(8,'1-1571983-0','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','2-1571999-6','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','Tyco Electronics','2-1571999-6','ADP0604','ALCO ADP','8 поз','20 В','','-30','+85','','',2008,'2014-02-28 14:50:05','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\ENG_CD_1571999_D4.pdf',2,0,'DIP-переключатель 2-1571999-6','Tyco',0,'',0,'8',27,NULL,NULL),(9,'FSM Series','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','1825910-5/6/7/8','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','Tyco Electronics','1825910-7','1825910-7','','50 мА','16 В','','-40','+125','','',2008,'2014-02-28 14:50:41','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\ENG_CD_1825910_C7.pdf',2,0,'Кнопка тактовая 1825910-7','',0,'',0,'50',19,NULL,NULL),(10,'1825006-7','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','1825006-7','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','Tyco Electronics','1825006-7','GDS04','','4 поз','24 В','','-45','+100','','',2008,'2014-03-11 15:48:09','1825006-7','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\ENG_CD_1825006_P.pdf',1,0,'DIP - переключатель','Tyco',0,'',1,'4',27,NULL,NULL),(11,'1571983-1','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','1571983-1','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','Tyco Electronics','1571983-1','GDH02S04','','2 поз','24 В','','-30','+85','','',2008,'2014-04-03 17:16:39','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\ENG_CD_1571983_C3.pdf',3,0,'DIP-переключатель SMD 2 поз, 1,27мм','1571983-1',0,'',1,'2',27,NULL,NULL),(12,'FSM Series','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','7914G-1-000E','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','Bourns Inc','7914G-1-000E','7914G-1-000','','100 мА','16 В','','-55','125','','',2008,'2014-04-01 13:17:25','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\7914.pdf',1,0,'Тактовая кнопка SMD 7914G-1-000E','',0,'',1,'100',19,NULL,NULL),(13,'1571983-1','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','1825006-3/4','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','Tyco Electronics','1825006-3','GDS0204','','2 поз','24 В','','-40','+85','','',2008,'2014-05-22 14:41:35','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\ENG_CD_1825006_P.pdf',1,0,'DIP-переключатель SMD 2 поз, 2.54мм','1825006-3(4), DMHA-02G-G, SWD4-2, DS1040-01-2',0,'',1,'2',27,NULL,NULL),(14,'ESP','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','ESP20','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','НКП','Переключатель угловой ESP-2010','','','2 поз','24 В','','-40','+85','','',2008,'2014-05-30 14:55:35','0,2 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\esp.pdf',1,0,'Переключатель угловой ESP-2010','ECE ESP-2010',0,'Переключатель углово',0,'2',27,NULL,NULL),(15,'ESP','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','MTS102C4','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','НКП','Тумблер угловой двухпозиционный MTS102-C4','','','1 кан','250 В','','-40','85','','',2008,'2015-04-20 13:59:54','3 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\mts102.pdf',1,1,'Тумблер угловой двухпозиционный MTS102-C4','MTS102-C4',0,'Тумблер угловой двух',0,'1',28,NULL,NULL),(16,'Button','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\switches.SchLib','TS-A4PV','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\switches.PcbLib','','','','','НКП','Тактовая кнопка угловая TS-A4PV','','','1 поз','12 В','','-40','85','','',2008,'2015-04-21 08:44:37','0,05 А','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Switches\\TS-A4PV.pdf',2,0,'Тактовая кнопка угловая TS-A4PV','TS-A4PV, 0795HIM-130G, SWT-3',0,'Тактовая кнопка угло',0,'1',27,NULL,NULL);
/*!40000 ALTER TABLE `switches` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `symbols`
--

DROP TABLE IF EXISTS `symbols`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `symbols` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `symbols`
--

LOCK TABLES `symbols` WRITE;
/*!40000 ALTER TABLE `symbols` DISABLE KEYS */;
INSERT INTO `symbols` VALUES (2,'AGND','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\symbols.SchLib','','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\symbols.PcbLib','','','','','Analog Devices','AGND','','','','','','','','','',2001,'2014-01-15 12:59:23',NULL,'',1,0,'AGND','',0,'',NULL,NULL,NULL,NULL,NULL),(3,'DGND','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\symbols.SchLib','','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\symbols.PcbLib','','','','','Analog Devices','DGND','','','','','','','','','',2001,'2014-01-15 12:59:37',NULL,'',1,0,'DGND','',0,'',NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `symbols` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transform`
--

DROP TABLE IF EXISTS `transform`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `transform` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transform`
--

LOCK TABLES `transform` WRITE;
/*!40000 ALTER TABLE `transform` DISABLE KEYS */;
INSERT INTO `transform` VALUES (5,'Current Transformer','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\transform.SchLib','TZ77','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\transform.PcbLib','','','','','НКП','TZ77V','','','1:2500 ','82 А','','-40','125','','',2008,'2014-07-07 13:50:25','130 Ом','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Transformers\\taehwatrans TZ77V.pdf',2,0,'TZ77V','Taehwatrans',0,'TZ77V',0,'1:2500',0,NULL,NULL),(6,'Transformer','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\transform.SchLib','TZ111','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\transform.PcbLib','','','','','НКП','TZ111V','','','1:1 ','','','-40','125','','',2008,'2014-06-27 12:46:02','100 Ом','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Transformers\\TZ111V_TL111V_TZ112V.pdf',1,0,'TZ111V','Taehwatrans',0,'TZ111V',0,'1:1',0,NULL,NULL);
/*!40000 ALTER TABLE `transform` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transistors`
--

DROP TABLE IF EXISTS `transistors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `transistors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=9 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transistors`
--

LOCK TABLES `transistors` WRITE;
/*!40000 ALTER TABLE `transistors` DISABLE KEYS */;
INSERT INTO `transistors` VALUES (1,'N-Ch Transistor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\transistors.SchLib','TO-263AB (D2PAK)','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\transistors.PcbLib','','','','','Fairchild Semiconductor','FDB3632','TO-263AB','FDB3632','12 А','100 В','','-55','125','1,16 Вт','2.07 Вт/C',2001,'2014-02-28 14:48:58','FDB3632','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Transistors\\Fairchild\\FDB3632.pdf',4,0,'Транзистор n-канальный FDB3632','Fairchild',0,'',1,'12',18,NULL,NULL),(5,'P-Ch transistor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\transistors.SchLib','SOT23','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\transistors.PcbLib','','','','','Fairchild Semiconductor','FDN338P','SOT23','.338','1,6 А','-20 В','','-50','+85','0,26 Вт','',2001,'2014-02-28 14:47:54','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Transistors\\Fairchild\\fdn338p.pdf',4,0,'Транзистор p-канальный FDN338P','Fairchild',0,'',1,'1,6',18,NULL,NULL),(6,'NPN','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\transistors.SchLib','SOT23','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\transistors.PcbLib','','','','','Fairchild Semiconductor','BC847A','SOT-23','8BA','100 мА','45 В','','-65','+150','0,31 Вт','',2001,'2014-07-22 12:48:21','200','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Transistors\\Fairchild\\bc847.pdf',1,0,'NPN-транзистор BC847A','Fairchild',0,'',1,'100',19,NULL,NULL),(7,'NPN','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\transistors.SchLib','SOT-223','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\transistors.PcbLib','','','','','NXP','BCP56','SOT-223','BCP56','1 А','80 В','','-65','150','1 Вт','',2008,'2014-08-13 17:23:54','150','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Transistors\\NXP\\BCP56_BCX56_BC56PA.pdf',1,0,'NPN-транзистор BCP56','NXP',0,'',1,'1',18,NULL,NULL),(8,'NPN','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\transistors.SchLib','SOT-223','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\transistors.PcbLib','','','','','ON Semiconductor','BCP68T1','SOT-223','BCP68','1 А','25 В','','-65','150','','1,5 Вт',2008,'2014-11-11 17:17:25','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Transistors\\ON Semiconductor\\BCP68T1-D.pdf',1,0,'NPN-транзистор BCP68T1','ON Semiconductor',0,'',1,'1',18,NULL,NULL);
/*!40000 ALTER TABLE `transistors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tvs`
--

DROP TABLE IF EXISTS `tvs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tvs` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=13 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tvs`
--

LOCK TABLES `tvs` WRITE;
/*!40000 ALTER TABLE `tvs` DISABLE KEYS */;
INSERT INTO `tvs` VALUES (1,'TVS-A','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\tvs.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\tvs.PcbLib','','','','','Littelfuse','SMBJ70A','DO-214AA (SMB)','NP','70 В','70 В','20%','-65','150','5','100 C/Вт',0,'2014-02-28 14:58:25','SMBJ70A','\\\\FSERVER\\PCAD\\Altium\\Libs\\DSheet\\TVS\\Littelfuse\\Littelfuse_TVS-Diode_SMBJ.pdf',2,0,'Защитный диод SMBJ70A','SMBT70A',0,'',1,'70',14,NULL,NULL),(8,'TVS-A','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\tvs.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\tvs.PcbLib','','','','','Littelfuse','SMBJ12A','DO-214AA (SMB)','LE','12 В','12 В','5%','-65','150','5','100 С/Вт',2008,'2014-02-28 14:58:12','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\TVS\\Littelfuse\\Littelfuse_TVS-Diode_SMBJ.pdf',2,0,'Защитный диод SMBJ12A','Littelfuse',0,'',1,'12',14,NULL,NULL),(9,'TVS-A','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\tvs.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\tvs.PcbLib','','','','','Littelfuse','SMBJ5.0A','DO-214AA','KE','5 В','','','-65','150','5 Вт','100C/Вт',2008,'2014-05-19 16:11:12','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\TVS\\Littelfuse\\Littelfuse_TVS-Diode_SMBJ.pdf',2,0,'Защитный диод однонаправленный SMBJ5.0A','SMBJ5.0A',0,'',1,'5',14,NULL,NULL),(10,'TVS-A','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\tvs.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\tvs.PcbLib','','','','','Littelfuse','SMBJ9.0A','DO-214AA','KV','9 В','','','-65','150','5 Вт','100С/Вт',2008,'2014-05-19 16:10:56','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\TVS\\Littelfuse\\Littelfuse_TVS-Diode_SMBJ.pdf',2,0,'Защитный диод однонаправленный SMBJ9.0A','SMBJ9.0A',0,'',1,'9',14,NULL,NULL),(11,'TVS-BiDir','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\tvs.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\tvs.PcbLib','','','','','Littelfuse','SMBJ13CA','DO-214AA','BG','13 В','','','-65','150','5 Вт','100 C/Вт',2008,'2014-05-19 16:46:06','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\TVS\\Littelfuse\\Littelfuse_TVS-Diode_SMBJ.pdf',1,0,'Диод защитный двунаправленный SMBJ13CA','SMBJ13CA',0,'',1,'13',14,NULL,NULL),(12,'TVS-BiDir','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\tvs.SchLib','DO-214AA (SMB) A','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\tvs.PcbLib','','','','','Littelfuse','SMBJ36CA','DO-214AA','CP','36 В','','','-65','150','5 Вт','100 С/Вт',2008,'2014-12-03 14:59:27','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\TVS\\Littelfuse\\Littelfuse_TVS-Diode_SMBJ.pdf',1,0,'SMBJ36CA','Littlefuse',0,'',1,'36',14,NULL,NULL);
/*!40000 ALTER TABLE `tvs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `varistors`
--

DROP TABLE IF EXISTS `varistors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `varistors` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Library Ref` varchar(45) NOT NULL,
  `Library Path` varchar(256) NOT NULL,
  `Footprint Ref` varchar(45) NOT NULL,
  `Footprint Path` varchar(256) NOT NULL,
  `Sim Description` varchar(45) DEFAULT NULL,
  `Sim File` varchar(256) DEFAULT NULL,
  `Sim Model Name` varchar(45) DEFAULT NULL,
  `Sim Parameters` varchar(45) DEFAULT NULL,
  `Manufacturer` varchar(45) NOT NULL,
  `PartNumber` varchar(64) NOT NULL,
  `Package` varchar(15) DEFAULT NULL,
  `Marking` varchar(10) DEFAULT NULL,
  `NominalValue` varchar(10) DEFAULT NULL,
  `NominalVoltage` varchar(10) DEFAULT NULL,
  `Tolerance` varchar(5) DEFAULT NULL,
  `OpTemperaturen` varchar(5) DEFAULT NULL,
  `OpTemperaturem` varchar(5) DEFAULT NULL,
  `Pmax` varchar(15) DEFAULT NULL,
  `TC` varchar(10) DEFAULT NULL,
  `Creator` int(11) NOT NULL,
  `Modify Date` datetime DEFAULT NULL,
  `Comment` varchar(45) DEFAULT NULL,
  `HelpURL` varchar(512) DEFAULT NULL,
  `RevNotes` int(11) DEFAULT NULL,
  `Discontinued` tinyint(1) NOT NULL,
  `Description` varchar(64) NOT NULL,
  `Notes` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) NOT NULL,
  `prefix` varchar(20) DEFAULT NULL,
  `isSMD` tinyint(1) DEFAULT NULL,
  `Nominal` varchar(8) DEFAULT NULL,
  `Unit` int(11) DEFAULT NULL,
  `par4` varchar(10) DEFAULT NULL,
  `par5` varchar(10) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=12 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `varistors`
--

LOCK TABLES `varistors` WRITE;
/*!40000 ALTER TABLE `varistors` DISABLE KEYS */;
INSERT INTO `varistors` VALUES (1,'Varistor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\varistors.SchLib','SIOV-S14K250','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\varistors.PcbLib','','','','','Epcos AG','B72214S0251K101','S14K250','','250 В','320 В','','-40','+85','0.6 Вт','',2001,'2014-02-28 14:57:48','S14K250','\\\\fserver\\pcad\\Altium\\Libs\\DSheet\\Varistors\\Epcos\\SIOV_Leaded_StandarD.pdf',3,0,'Варистор SIOV-S14K250','Epcos',0,'',0,'250',14,NULL,NULL),(9,'Varistor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\varistors.SchLib','SIOV-S20K11','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\varistors.PcbLib','','','','','Epcos AG','B72220S0500K101','S20K50','','50 В','65 В','','-40','+85','1 Вт','',2008,'2014-02-28 14:57:56','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Varistors\\Epcos\\SIOV_Leaded_StandarD.pdf',2,0,'Варистор SIOV-S20K50','Epcos',0,'',0,'50',14,NULL,NULL),(10,'Varistor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\varistors.SchLib','SIOV-S14K275','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\varistors.PcbLib','','','','','Epcos AG','B72214S0271K101','S14K275','','275 В','350 В','','-40','+85','0','',2008,'2014-06-04 17:41:56','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Varistors\\Epcos\\SIOV_Leaded_StandarD.pdf',1,0,'Варисто SIOV-S14K275','Epcos',0,'',0,'275',14,NULL,NULL),(11,'Varistor','\\\\Fserver\\pcad\\Altium\\Libs\\Symbols\\varistors.SchLib','SIOV-S14K420','\\\\Fserver\\pcad\\Altium\\Libs\\Footprints\\varistors.PcbLib','','','','','Epcos AG','B72214S0421K101','S14K420','','420 В','560 В','','-40','+85','0,6 Вт','',2008,'2014-06-04 18:34:33','','\\\\Fserver\\pcad\\Altium\\Libs\\DSheet\\Varistors\\Epcos\\SIOV_Leaded_StandarD.pdf',1,0,'Варистор SIOV-S14K420','Epcos',0,'',0,'420',14,NULL,NULL);
/*!40000 ALTER TABLE `varistors` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-02-11 14:13:28
