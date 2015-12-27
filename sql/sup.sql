-- MySQL dump 10.13  Distrib 5.6.22, for Win64 (x86_64)
--
-- Host: localhost    Database: supik
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
-- Table structure for table `banks`
--

DROP TABLE IF EXISTS `banks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `banks` (
  `idbanks` int(11) NOT NULL,
  `banks` varchar(128) DEFAULT NULL,
  `address` varchar(256) DEFAULT NULL,
  `phone` varchar(45) DEFAULT NULL,
  `www` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idbanks`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `banks`
--

LOCK TABLES `banks` WRITE;
/*!40000 ALTER TABLE `banks` DISABLE KEYS */;
/*!40000 ALTER TABLE `banks` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `chooselists`
--

DROP TABLE IF EXISTS `chooselists`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `chooselists` (
  `idchooselists` int(11) NOT NULL,
  `chooselists` varchar(45) DEFAULT NULL,
  `table` varchar(45) DEFAULT NULL,
  `tablefield` varchar(45) DEFAULT NULL,
  `header` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idchooselists`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `chooselists`
--

LOCK TABLES `chooselists` WRITE;
/*!40000 ALTER TABLE `chooselists` DISABLE KEYS */;
/*!40000 ALTER TABLE `chooselists` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `countries`
--

DROP TABLE IF EXISTS `countries`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `countries` (
  `idcountries` int(11) NOT NULL,
  `countries` varchar(45) DEFAULT NULL,
  `abbr` varchar(4) DEFAULT NULL,
  PRIMARY KEY (`idcountries`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `countries`
--

LOCK TABLES `countries` WRITE;
/*!40000 ALTER TABLE `countries` DISABLE KEYS */;
/*!40000 ALTER TABLE `countries` ENABLE KEYS */;
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
  `deleted` varchar(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`iddescription`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `description`
--

LOCK TABLES `description` WRITE;
/*!40000 ALTER TABLE `description` DISABLE KEYS */;
/*!40000 ALTER TABLE `description` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dialogmethods`
--

DROP TABLE IF EXISTS `dialogmethods`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dialogmethods` (
  `iddialogmethods` int(11) NOT NULL,
  `dialogmethods` varchar(45) DEFAULT NULL,
  `dialog` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddialogmethods`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dialogmethods`
--

LOCK TABLES `dialogmethods` WRITE;
/*!40000 ALTER TABLE `dialogmethods` DISABLE KEYS */;
INSERT INTO `dialogmethods` VALUES (1,'accept()','ncdialog'),(2,'close()','ncdialog'),(3,'filter()','ncdialog'),(4,'new()','ncdialog'),(5,'refresh()','ncdialog'),(6,'search()','ncdialog');
/*!40000 ALTER TABLE `dialogmethods` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dialogs`
--

DROP TABLE IF EXISTS `dialogs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dialogs` (
  `iddialogs` int(11) NOT NULL,
  `dialogs` varchar(45) DEFAULT NULL,
  `keyfield` varchar(1) DEFAULT NULL,
  `dialogsfields` varchar(45) DEFAULT NULL,
  `dialogslinks` varchar(128) DEFAULT NULL,
  `fieldslocation` varchar(45) DEFAULT NULL,
  `field` varchar(45) DEFAULT NULL,
  `fieldasc` varchar(3) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  `default` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddialogs`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dialogs`
--

LOCK TABLES `dialogs` WRITE;
/*!40000 ALTER TABLE `dialogs` DISABLE KEYS */;
INSERT INTO `dialogs` VALUES (1,'whincome','','Приходный ордер','11','30',NULL,'00',NULL,'0',NULL,NULL),(2,'whincome','v','ent.documents.iddocuments','12','31',NULL,'02',NULL,'0',NULL,NULL),(3,'whincome',NULL,'Номер ордера','11','31',NULL,'01',NULL,'0',NULL,NULL),(4,'whincome',NULL,'от','11','31',NULL,'03',NULL,'0',NULL,NULL),(5,'whincome',NULL,'ent.documents.date','2.18.','31',NULL,'04',NULL,'0',NULL,'$date'),(6,'whincome',NULL,'Записать и закрыть','9..this_accept.1.4','35',NULL,'05',NULL,'0',NULL,NULL),(7,'whincome',NULL,'Отменить','9..this_close.2.3','35',NULL,'06',NULL,'0',NULL,NULL),(12,'whincome',NULL,'','13.900','35',NULL,'07',NULL,'0',NULL,NULL),(10,'whincome',NULL,'Поставщик:','11','32.0',NULL,'12',NULL,'0',NULL,NULL),(11,'whincome',NULL,'ent.documents.idsupplier','2.2..ent.contractors.contractors','32.0',NULL,'13',NULL,'0',NULL,NULL),(13,'whincome',NULL,'Автор:','11','35',NULL,'08',NULL,'0',NULL,NULL),(14,'whincome',NULL,'Персонал.ФИО','0','35',NULL,'09',NULL,'0',NULL,'$pers'),(15,'whincome',NULL,'Получатель:','11','32.0',NULL,'14',NULL,'0',NULL,NULL),(16,'whincome',NULL,'ent.documents.idcustomer','2.2..ent.contractors.contractors','32.0',NULL,'15',NULL,'0',NULL,NULL);
/*!40000 ALTER TABLE `dialogs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dirlist`
--

DROP TABLE IF EXISTS `dirlist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dirlist` (
  `iddirlist` int(11) NOT NULL AUTO_INCREMENT,
  `dirlist` varchar(45) DEFAULT NULL,
  `access` varchar(45) DEFAULT NULL,
  `dirparent` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddirlist`),
  UNIQUE KEY `idtreealiases_UNIQUE` (`iddirlist`)
) ENGINE=MyISAM AUTO_INCREMENT=26 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dirlist`
--

LOCK TABLES `dirlist` WRITE;
/*!40000 ALTER TABLE `dirlist` DISABLE KEYS */;
INSERT INTO `dirlist` VALUES (3,'Номенклатура','01FF','Категории',0,NULL,NULL),(4,'Производители','4F',NULL,0,'2001','2015-12-25 22:07:14'),(6,'Объекты','004F',NULL,0,NULL,NULL),(7,'Персонал','6',NULL,0,NULL,NULL),(8,'Единицы измерения','0007',NULL,0,NULL,NULL),(9,'Категории','0007',NULL,0,NULL,NULL),(10,'Контрагенты','1FF',NULL,0,NULL,NULL),(11,'Основания','7',NULL,0,NULL,NULL),(12,'Наборы параметров','1FF',NULL,0,NULL,NULL),(14,'Валюты','7',NULL,0,NULL,NULL),(13,'Документы','36DF',NULL,0,'2001','2015-12-06 22:32:54'),(15,'Поля ордеров','7',NULL,0,NULL,NULL),(16,'Периодические','0007',NULL,0,NULL,NULL),(17,'Движения номенклатуры','0007',NULL,1,NULL,NULL),(2,'Склады размещение','127F',NULL,0,'2001','2015-10-09 15:48:16'),(19,'Номенклатура карантин','000F','Категории',0,'2001',NULL),(20,'Компоненты Altium (к)','0007',NULL,1,'2001',NULL),(1,'Группы доступа','6',NULL,0,NULL,NULL),(18,'Склады типы размещения','127F',NULL,0,'2001','2015-10-05 21:50:23'),(22,'Склады ёмкости размещения','7',NULL,0,NULL,NULL),(5,'Классы документов','CF',NULL,0,'2001','2015-12-06 20:44:57'),(21,'Перечни','1CF',NULL,0,'2001','2015-11-08 15:51:56'),(23,'Изделия','1CF',NULL,0,'2001','2015-12-25 21:02:47'),(24,'Документация','13DF',NULL,0,'2001','2015-12-06 20:58:51'),(25,'Классификатор ЕСКД','555',NULL,0,'2001','2015-12-27 13:12:50');
/*!40000 ALTER TABLE `dirlist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dirsyslist`
--

DROP TABLE IF EXISTS `dirsyslist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dirsyslist` (
  `iddirsyslist` int(11) NOT NULL AUTO_INCREMENT,
  `dirsyslist` varchar(45) DEFAULT NULL,
  `access` varchar(45) DEFAULT NULL,
  `dirparent` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`iddirsyslist`),
  UNIQUE KEY `iddirsyslist_UNIQUE` (`iddirsyslist`)
) ENGINE=MyISAM AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dirsyslist`
--

LOCK TABLES `dirsyslist` WRITE;
/*!40000 ALTER TABLE `dirsyslist` DISABLE KEYS */;
INSERT INTO `dirsyslist` VALUES (1,'Методы главного меню','0007',NULL,0,NULL,NULL),(2,'Методы системного меню','0007',NULL,0,NULL,NULL);
/*!40000 ALTER TABLE `dirsyslist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `flowfields`
--

DROP TABLE IF EXISTS `flowfields`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `flowfields` (
  `idflowfields` int(11) NOT NULL,
  `flowfields` varchar(128) DEFAULT NULL,
  `flowheaders` varchar(128) DEFAULT NULL,
  `links` varchar(128) DEFAULT NULL,
  `flowfieldsasc` varchar(128) DEFAULT NULL,
  `date` datetime DEFAULT NULL,
  `deleted` int(1) NOT NULL DEFAULT '0',
  `idpers` varchar(128) DEFAULT NULL,
  PRIMARY KEY (`idflowfields`),
  UNIQUE KEY `idflowfields_UNIQUE` (`idflowfields`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `flowfields`
--

LOCK TABLES `flowfields` WRITE;
/*!40000 ALTER TABLE `flowfields` DISABLE KEYS */;
INSERT INTO `flowfields` VALUES (1,'','Наименование столбца','','Порядок следования полей','0000-00-00 00:00:00',0,NULL),(2,'Имя поля в flow','Наименование столбца','Описание поля','Порядок следования полей','0000-00-00 00:00:00',0,NULL),(3,'-','№','0.4','00','0000-00-00 00:00:00',0,'2001'),(4,'links','Наименование','3.2..ent.qnk.ent.nk','01','0000-00-00 00:00:00',0,'2001'),(5,'quantity','Кол.','6.5..nnnn','02','0000-00-00 00:00:00',0,'2001'),(6,'links','Ед. изм.','4.3..ent.unitset.Номенклатура','03','0000-00-00 00:00:00',0,'2001'),(7,'price','Цена б/НДС','6.4..\"^[0-9]{1,9}[.]{0,1}[0-9]{0,2}$\"','04','0000-00-00 00:00:00',0,'2001'),(8,'-','НДС','5.4..ent.periodic.periodic.НДС.value','07','0000-00-00 00:00:00',0,'2001'),(9,'-','Стоимость','7.4..s.r.c2*c4*c6.c2*c4*c6*c7/100','08','0000-00-00 00:00:00',0,'2001'),(10,'links','Валюта','2.3..ent.currency.abbreviation','05','0000-00-00 00:00:00',0,'2001'),(11,'links','Склад','2.3..ent.wh.wh','09',NULL,0,'2001'),(12,'-','Курс','5.4.5.ent.periodic.periodic.value','06',NULL,0,'2001');
/*!40000 ALTER TABLE `flowfields` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `groups`
--

DROP TABLE IF EXISTS `groups`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `groups` (
  `idgroups` int(11) NOT NULL,
  `groups` varchar(45) DEFAULT NULL,
  `access` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idgroups`)
) ENGINE=MyISAM AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `groups`
--

LOCK TABLES `groups` WRITE;
/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
INSERT INTO `groups` VALUES (2001,'Администратор','0FFF',0,'2001','2015-11-26 21:28:05'),(2002,'Менеджер Altium','0030',0,NULL,NULL),(2003,'Менеджер склада','000C',0,NULL,NULL),(2004,'Управляющий Altium','0034',0,NULL,NULL),(2005,'Управляющий склада','001C',0,NULL,NULL),(2006,'Конструктор','0C15',0,NULL,NULL);
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `indexes`
--

DROP TABLE IF EXISTS `indexes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `indexes` (
  `idindexes` int(11) NOT NULL,
  `indexes` varchar(45) DEFAULT NULL,
  `reg` varchar(128) DEFAULT NULL,
  `area` varchar(128) DEFAULT NULL,
  `city` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idindexes`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `indexes`
--

LOCK TABLES `indexes` WRITE;
/*!40000 ALTER TABLE `indexes` DISABLE KEYS */;
/*!40000 ALTER TABLE `indexes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mainmenu`
--

DROP TABLE IF EXISTS `mainmenu`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mainmenu` (
  `idmainmenu` int(11) NOT NULL AUTO_INCREMENT,
  `alias` varchar(45) DEFAULT NULL,
  `idalias` varchar(45) DEFAULT NULL,
  `order` varchar(2) DEFAULT NULL,
  `access` varchar(45) DEFAULT NULL,
  `tooltip` varchar(64) DEFAULT NULL,
  `method` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  `date` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idmainmenu`)
) ENGINE=MyISAM AUTO_INCREMENT=39 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mainmenu`
--

LOCK TABLES `mainmenu` WRITE;
/*!40000 ALTER TABLE `mainmenu` DISABLE KEYS */;
INSERT INTO `mainmenu` VALUES (5,'Справочники','0','01','3F','Справочники','',0,'2015-12-16 22:02:19','2001'),(6,'Компоненты','0','03','4F','','',0,'2015-12-16 22:02:35','2001'),(3,'Супик','0','00','7FFF','','1',0,'2015-12-16 22:01:00','2001'),(4,'Выход','3','','555','Выход из программы','1',0,'2015-12-17 13:29:54','2001'),(8,'Склады','0','04','127F','','',0,'2015-12-16 22:02:41','2001'),(9,'Документы','0',NULL,'1','','8',1,'2015-09-24 21:20:57','2001'),(10,'Сисадминистрирование','0','06','303','Работа системного администратора','8',0,'2015-12-17 13:17:19','2001'),(11,'Уголок ТБ','0','07','1','','8',0,'2015-12-16 22:03:04','2001'),(12,'Объекты','0','05','1','','8',0,'2015-12-16 22:02:46','2001'),(13,'Система','0','08','555','Редактирование системных настроек','8',0,'2015-12-17 13:12:31','2001'),(14,'Помощь','0','11','555','','8',0,'2015-12-17 13:31:56','2001'),(15,'&Редактирование структуры системы','13',NULL,'7','Редактор системных элементов - меню, деревьев и т.п.','2',1,NULL,NULL),(7,'&Настройки','13',NULL,'1','Настройки системы','SettingsEdit',1,NULL,NULL),(26,'Карантин','0','09','3F','Работа с карантинными справочниками','8',0,'2015-12-16 22:03:20','2001'),(16,'Внимание!','0','10','7FFF','Есть проблемы!','8',0,'2015-12-16 22:03:29','2001'),(17,'Принять на склад','32','','F','Приём комплектующих на склад','10',0,'2015-12-17 13:17:58','2001'),(18,'Выдать со склада','8',NULL,'3F','Списывание комплектующих со склада','8',0,'2015-09-24 21:28:56','2001'),(19,'Поиск...','8',NULL,'7FFF','Поиск комплектующих по складам','8',0,'2015-09-24 21:29:07','2001'),(20,'Работа со справочниками','5','','555','Работа со справочниками','4',0,'2015-12-17 13:30:44','2001'),(21,'Экспорт в файл','5',NULL,'17','Экспорт содержимого справочника в файл','8',0,'2015-09-24 21:29:28','2001'),(22,'&Импорт из файла','5',NULL,'17','Импорт справочника из файла','8',0,'2015-09-24 21:29:35','2001'),(23,'&Конфигуратор диалогов','13',NULL,'0007','Конфигуратор диалоговых окон',NULL,1,NULL,'2001'),(24,'Диалог с &2 полями','23',NULL,'0007','Редактирование диалогов вида Параметр=Значение','DialogEdit',1,NULL,'2001'),(25,'&Диалог общего вида','23',NULL,'0007','Редактирование диалогов общего вида','DialogEdit',1,NULL,'2001'),(1,' Справочники','13',NULL,'7','','',1,'02/07/2015 18:11:13','2001'),(2,'Системные справочники','1',NULL,'7','Работа с системными справочниками','5',1,'02/07/2015 18:11:13','2001'),(27,'Экспорт','1',NULL,'7','Экспорт справочников в файл','',1,'02/07/2015 18:11:13','2001'),(28,'Импорт','1',NULL,'7','Импорт справочников из файла','',1,'02/07/2015 18:11:13','2001'),(29,'Altium','6',NULL,'1CF','','',0,'06/07/2015 20:45:21','2001'),(30,'Редактор','29','','11','Редактирование компонентов','7',0,'2015-12-17 13:30:19','2001'),(31,'Карантин','29','','22','Просмотр текущих изменений БД, находящихся на одобрении','8',0,'2015-12-17 13:30:31','2001'),(32,'Склады','8',NULL,'7','Редактор складов','9',0,'2015-11-08 11:37:12','2001'),(33,'Продукция','0','02','1FF','Выпускаемая продукция','8',0,'2015-12-16 22:02:28','2001'),(34,'Документы','33','','555','Редактирование документов на изделия','11',0,'2015-12-17 13:31:46','2001'),(35,'Классификатор','33','','555','Классификатор изделий','12',0,'2015-12-17 13:31:19','2001'),(36,'Настройки','13','1','555','Настройки системы','3',0,'2015-12-17 13:10:37','2001'),(37,'Редактор','13','0','3','Редактор структуры системы','2',0,'2015-12-17 13:17:38','2001'),(38,'Импорт классификатора ЕСКД','13','','2','Импорт классификатора ЕСКД','13',0,'2015-12-26 13:58:53','2001');
/*!40000 ALTER TABLE `mainmenu` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mainmenumethods`
--

DROP TABLE IF EXISTS `mainmenumethods`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mainmenumethods` (
  `idmainmenumethods` int(11) NOT NULL,
  `mainmenumethods` varchar(45) DEFAULT NULL,
  `description` varchar(256) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` varchar(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idmainmenumethods`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mainmenumethods`
--

LOCK TABLES `mainmenumethods` WRITE;
/*!40000 ALTER TABLE `mainmenumethods` DISABLE KEYS */;
INSERT INTO `mainmenumethods` VALUES (1,'ExitSupik','Выход из программы','02/07/2015 21:40:15','2001','0'),(2,'SysStructEdit','Редактирование структуры системы',NULL,NULL,'0'),(3,'SettingsEdit','Редактирование настроек',NULL,NULL,'0'),(4,'Directories','Редактирование справочников',NULL,NULL,'0'),(5,'SysDirectories','Редактор системных справочников',NULL,NULL,'0'),(6,NULL,NULL,NULL,NULL,'1'),(7,'Components','Работа с компонентами','06/07/2015 20:45:21','2001','0'),(8,'Dummy','Пустышка','2015-09-24 21:18:03','2001','0'),(9,'WhEditor','Редактор складов','2015-09-26 14:51:15','2001','0'),(10,'WhIncome','Приход на склад','2015-09-26 15:42:48','2001','0'),(11,'DevDoc','Документы на изделия','2015-11-08 15:23:41','2001','0'),(12,'DevDev','Классификатор изделий','2015-12-12 23:11:48','2001','0'),(13,'SysImportClass','Импорт классификатора','2015-12-26 13:58:05','2001','0');
/*!40000 ALTER TABLE `mainmenumethods` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `messages`
--

DROP TABLE IF EXISTS `messages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `messages` (
  `idmessages` int(11) NOT NULL,
  `messages` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `topers` varchar(45) DEFAULT NULL,
  `ackreq` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idmessages`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `messages`
--

LOCK TABLES `messages` WRITE;
/*!40000 ALTER TABLE `messages` DISABLE KEYS */;
/*!40000 ALTER TABLE `messages` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `personel`
--

DROP TABLE IF EXISTS `personel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `personel` (
  `idpersonel` int(11) NOT NULL AUTO_INCREMENT,
  `personel` varchar(45) DEFAULT NULL,
  `login` varchar(45) DEFAULT NULL,
  `psw` varchar(45) DEFAULT NULL,
  `group` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT '0',
  `date` datetime DEFAULT NULL,
  PRIMARY KEY (`idpersonel`),
  UNIQUE KEY `idpersonel_UNIQUE` (`idpersonel`)
) ENGINE=MyISAM AUTO_INCREMENT=2013 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `personel`
--

LOCK TABLES `personel` WRITE;
/*!40000 ALTER TABLE `personel` DISABLE KEYS */;
INSERT INTO `personel` VALUES (2001,'Валуйских А.О.','vao','evel_489751','2001','0',0,NULL),(2008,'Костюк М.В.','kmv','kmv_162886','2002','2001',0,'2014-01-20 13:33:38'),(2009,'Поволяев А.Л.','pal','pal_522068','2002','2001',0,'2014-02-12 15:31:32'),(2010,'Цфасман Г.М.','cgm','grig_835645','2002','2001',0,'2014-02-12 15:32:00'),(2011,'Жуков А.М.','zam','ghu_036819','2001','2001',0,'2014-02-28 16:22:44'),(3,'Васин М.Е.','vme','','2003','2001',0,NULL);
/*!40000 ALTER TABLE `personel` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `psw`
--

DROP TABLE IF EXISTS `psw`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `psw` (
  `idpsw` int(11) NOT NULL,
  `psw` varchar(45) DEFAULT NULL,
  `dept` varchar(2) DEFAULT NULL,
  PRIMARY KEY (`idpsw`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `psw`
--

LOCK TABLES `psw` WRITE;
/*!40000 ALTER TABLE `psw` DISABLE KEYS */;
/*!40000 ALTER TABLE `psw` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `syslist`
--

DROP TABLE IF EXISTS `syslist`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `syslist` (
  `idsyslist` int(11) NOT NULL,
  `alias` varchar(45) NOT NULL,
  `idalias` varchar(45) NOT NULL,
  `access` varchar(45) DEFAULT NULL,
  `pc` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `deleted` int(1) DEFAULT '0',
  PRIMARY KEY (`idsyslist`),
  UNIQUE KEY `idsyslist_UNIQUE` (`idsyslist`)
) ENGINE=MyISAM AUTO_INCREMENT=39 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `syslist`
--

LOCK TABLES `syslist` WRITE;
/*!40000 ALTER TABLE `syslist` DISABLE KEYS */;
INSERT INTO `syslist` VALUES (2001,'Работа с БД','0','1',NULL,NULL,NULL,1),(2002,'Добавить таблицу','2013','1',NULL,NULL,NULL,1),(2003,'Изменить структуру таблицы','2013','1',NULL,NULL,NULL,1),(2004,'Создание скелета БД','2001','1',NULL,NULL,NULL,1),(2005,'Заполнение','2001','1',NULL,NULL,NULL,1),(2006,'Работа с журналами...','0','1',NULL,NULL,NULL,1),(2007,'Резервное копирование','0','1',NULL,NULL,NULL,1),(2008,'Восстановление из резервной копии','2007','1',NULL,NULL,NULL,1),(2009,'...авторизаций','2006','1',NULL,NULL,NULL,1),(2010,'Создание резервной копии','2007','1',NULL,NULL,NULL,1),(2011,'...событий','2006','1',NULL,NULL,NULL,1),(2012,'Структура разделов номенклатуры','2001','1',NULL,NULL,NULL,1),(2013,'Таблицы...','2001','1',NULL,NULL,NULL,1),(2014,'Изменить содержимое таблицы','2013','1',NULL,NULL,NULL,1),(2015,'Синхронизация с основным сервером','2007','1',NULL,NULL,NULL,1),(2016,'altium','2014','1',NULL,NULL,NULL,1),(2017,'enterprise','2014','1',NULL,NULL,NULL,1),(2018,'Система','0','7','','2001','02/07/2015 14:28:10',0),(2019,'Редактор системного меню','2018','7','2','2001','02/07/2015 17:09:12',0),(2020,'Состав справочников','2018','7','5','2001','2015-10-04 16:38:47',0),(2023,'Компоненты','0','1',NULL,NULL,NULL,0),(2022,'Категории','2023','1','altium',NULL,NULL,0),(2024,'Восстановление удалённых','2001','1',NULL,NULL,NULL,1),(2026,'Редактор главного меню','2018','7','1','2001','02/07/2015 18:01:11',0),(3,'Резервное копирование','2018','7','','2001','02/07/2015 17:09:12',0),(1,'','',NULL,NULL,NULL,NULL,0),(2,'Справочники','0','7','','2001','02/07/2015 17:19:06',0),(4,'Системные справочники','2','7','3','2001','18/07/2015 12:51:59',0),(5,'Экспорт','2','7','','2001','02/07/2015 17:19:06',0),(6,'Импорт','2','7','','2001','02/07/2015 17:19:06',0),(7,'Таблицы','0','7','4','2001','18/07/2015 13:45:53',0),(8,'Редактор','7','7','','2001','02/07/2015 17:19:06',1);
/*!40000 ALTER TABLE `syslist` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sysmenumethods`
--

DROP TABLE IF EXISTS `sysmenumethods`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sysmenumethods` (
  `idsysmenumethods` int(11) NOT NULL,
  `sysmenumethods` varchar(45) DEFAULT NULL,
  `description` varchar(256) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `idpers` varchar(45) DEFAULT NULL,
  `deleted` varchar(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`idsysmenumethods`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sysmenumethods`
--

LOCK TABLES `sysmenumethods` WRITE;
/*!40000 ALTER TABLE `sysmenumethods` DISABLE KEYS */;
INSERT INTO `sysmenumethods` VALUES (1,'mainmenueditor','Редактор главного меню',NULL,NULL,'0'),(2,'sysmenueditor','Редактор системного меню',NULL,NULL,'0'),(3,'sysdireditor','Редактор системных справочников','18/07/2015 12:01:59','2001','0'),(4,'tableseditor','Редактор системных таблиц','18/07/2015 13:45:53','2001','0'),(5,'direditor','Редактор справочников','2015-10-04 16:34:09','2001','0');
/*!40000 ALTER TABLE `sysmenumethods` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tablefields`
--

DROP TABLE IF EXISTS `tablefields`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `tablefields` (
  `idtablefields` int(11) NOT NULL,
  `tablefields` varchar(45) DEFAULT NULL,
  `table` varchar(45) DEFAULT NULL,
  `keyfield` varchar(2) DEFAULT NULL,
  `header` varchar(128) DEFAULT NULL,
  `links` varchar(512) DEFAULT NULL,
  `fieldsorder` varchar(3) DEFAULT NULL,
  `tablename` varchar(45) DEFAULT NULL,
  `date` varchar(45) DEFAULT NULL,
  `deleted` varchar(45) DEFAULT '0',
  `idpers` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idtablefields`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tablefields`
--

LOCK TABLES `tablefields` WRITE;
/*!40000 ALTER TABLE `tablefields` DISABLE KEYS */;
INSERT INTO `tablefields` VALUES (1,'idnk','ent.nk','v','ИД','4.19..7','00','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(2,'nk','ent.nk','','Наименование','1.8','01','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(3,'idclasses','ent.nk','','Категория','2.2..Категории_сокращ.Наименование','02','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(4,'idmanuf','ent.nk','','Производитель','2.2..Производители_сокращ.Наименование','03','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(5,'idalt','ent.nk','','Элемент в БД altium','2.11..АКомпоненты_описание_полн.Описание','04','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(6,'idsdw','ent.nk','','Элемент в БД solidworks','2.11..3Компоненты_описание.Описание','05','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(7,'idsge','ent.nk','','Элемент в БД schemagee','2.11..ЭКомпоненты_описание.Описание','06','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(8,'idctr','ent.nk','','Элемент в БД конструктивов','2.11..ККомпоненты_описание.Описание','07','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(9,'iddev','ent.nk','','Элемент в БД устройств','2.11..УКомпоненты_описание.Описание','08','Номенклатура_полн','20/08/2015 15:28:51','0','2001'),(10,'idclasses','ent.classes','v','ИД','4.19..7','00','Категории_сокращ',NULL,'0',NULL),(11,'alias','ent.classes',NULL,'Наименование','1.8','01','Категории_сокращ',NULL,'0',NULL),(12,'idalias','ent.classes',NULL,'ИД_а','2.4..Наименование','02','Категории_сокращ',NULL,'0',NULL),(13,'idmanuf','ent.manuf','v','ИД','4.19..7','00','Производители_сокращ','2015-12-25 22:07:14','0','2001'),(14,'manuf','ent.manuf','','Наименование','1.8..','01','Производители_сокращ','2015-12-25 22:07:14','0','2001'),(15,'idflow','ent.flow','v','ИД','4.19..7','00','Приём на склад_полн',NULL,'0',NULL),(16,'idnk','ent.flow',NULL,'Наименование','2.3..Номенклатура_сокращ.Номенклатура карантин_сокращ','01','Приём на склад_полн',NULL,'0',NULL),(17,'quantity','ent.flow',NULL,'Кол.','5.6..nnnn','02','Приём на склад_полн',NULL,'0',NULL),(18,'idunitset','ent.flow',NULL,'Ед. изм.','3.4..Единицы измерения_сокращ.Номенклатура','03','Приём на склад_полн',NULL,'0',NULL),(19,'price','ent.flow',NULL,'Цена б/НДС','1.6..\"^[0-9]{1,9}[.]{0,1}[0-9]{0,2}$\"','04','Приём на склад_полн',NULL,'0',NULL),(20,'-','-',NULL,'НДС','4.5..Периодические_полн.Значение.Наименование.НДС','06','Приём на склад_полн',NULL,'0',NULL),(21,'idcurrency','ent.flow',NULL,'Валюта','3.2..Валюта_сокращ.Сокращение валюты','05','Приём на склад_полн',NULL,'0',NULL),(22,'-','-',NULL,'Стоимость','4.7..s.c2*c4.c2*c4*c6/100','07','Приём на склад_полн',NULL,'0',NULL),(23,'idnkwh','ent.flow',NULL,'Склад','3.2..Склады_сокращ.Наименование','08','Приём на склад_полн',NULL,'0',NULL),(24,'idcontractors','ent.contractors','v','ИД','4.19..7','00','Контрагенты_сокращ','27/08/2015 20:07:06','1','2001'),(25,'contractors','ent.contractors','','Наименование','1.8..','01','Контрагенты_сокращ','27/08/2015 20:07:06','1','2001'),(26,'city','ent.contractors','','Город','1.8..','04','Контрагенты_сокращ','27/08/2015 20:07:06','1','2001'),(27,'iddirlist','sup.dirlist','v','ИД','4.19..7','00','Справочники_сокращ',NULL,'0',NULL),(28,'dirlist','sup.dirlist',NULL,'Наименование','1.8','01','Справочники_сокращ',NULL,'0',NULL),(29,'idnk','ent.nk','v','ИД','7.19..7','00','Номенклатура_сокращ',NULL,'0',NULL),(30,'nk','ent.nk',NULL,'Наименование','4.8','01','Номенклатура_сокращ',NULL,'0',NULL),(31,'idmanuf','ent.nk',NULL,'Производитель','7.2..Производители_сокращ.Наименование','02','Номенклатура_сокращ',NULL,'0',NULL),(32,'idqnk','ent.qnk','v','ИД','4.19..7','00','Номенклатура карантин_сокращ',NULL,'0',NULL),(33,'qnk','ent.qnk',NULL,'Наименование','4.8','01','Номенклатура карантин_сокращ',NULL,'0',NULL),(34,'idmanuf','ent.qnk',NULL,'Производитель','4.2..Производители_сокращ.Наименование','02','Номенклатура карантин_сокращ',NULL,'0',NULL),(35,'idclasses','ent.classes','v','ИД','4.19..7','00','Категории_полн','2015-09-19 13:29:09','0','2001'),(36,'alias','ent.classes','','Наименование','1.8..','01','Категории_полн','2015-09-19 13:29:09','0','2001'),(37,'idalias','ent.classes','','ИД_а','2.4..Категории_полн.Наименование','02','Категории_полн','2015-09-19 13:29:09','0','2001'),(151,'docclasses','ent.documents','','Класс документа','2.2..Классы документов_сокращ.Наименование','02','Документы_полн','2015-12-06 22:32:54','0','2001'),(42,'documents','ent.documents','','Наименование','1.8..','01','Документы_полн','2015-12-06 22:32:54','0','2001'),(43,'parameters','ent.classes','','Параметры','2.2..Наборы параметров_сокращ.Наименование','03','Категории_полн','2015-09-19 13:29:09','0','2001'),(44,'idparameters','ent.parameters','v','ИД','4.19..7','00','Наборы параметров_сокращ','30/08/2015 17:07:22','0','2001'),(45,'parameters','ent.parameters','','Наименование','1.8..','01','Наборы параметров_сокращ','30/08/2015 17:07:22','0','2001'),(46,'idqnk','ent.qnk','v','ИД','4.19..7','00','Номенклатура карантин_полн',NULL,'0',NULL),(47,'qnk','ent.qnk',NULL,'Наименование','4.8','01','Номенклатура карантин_полн',NULL,'0',NULL),(48,'idclasses','ent.qnk',NULL,'Категория','2.2..Категории_сокращ.Наименование','02','Номенклатура карантин_полн',NULL,'0',NULL),(49,'idmanuf','ent.qnk',NULL,'Производитель','2.2..Производители_сокращ.Наименование','03','Номенклатура карантин_полн',NULL,'0',NULL),(50,'idalt','ent.qnk',NULL,'Элемент в БД altium','2.11..Категории_полн.Altium','04','Номенклатура карантин_полн',NULL,'0',NULL),(51,'idsdw','ent.qnk',NULL,'Элемент в БД solidworks','2.11..Категории_полн.Solidworks','05','Номенклатура карантин_полн',NULL,'0',NULL),(52,'idsge','ent.qnk',NULL,'Элемент в БД schemagee','2.11..Категории_полн.Schemagee','06','Номенклатура карантин_полн',NULL,'0',NULL),(53,'idctr','ent.qnk',NULL,'Элемент в БД конструктивов','2.11..Категории_полн.Конструктивы','07','Номенклатура карантин_полн',NULL,'0',NULL),(54,'iddev','ent.qnk',NULL,'Элемент в БД устройств','2.11..Категории_полн.Изделия','08','Номенклатура карантин_полн',NULL,'0',NULL),(55,'iddescription','alt.description','v','ИД','4.19..7','00','АКомпоненты_описание_сокращ',NULL,'0',NULL),(56,'description','alt.description',NULL,'Наименование','4.8','01','АКомпоненты_описание_сокращ',NULL,'0',NULL),(57,'descriptionfull','alt.description',NULL,'Описание','4.8','02','АКомпоненты_описание_сокращ',NULL,'0',NULL),(58,'idsyslist','sup.syslist','v','ИД','4.19..7','00','Системное меню_сокращ',NULL,'0',NULL),(59,'alias','sup.syslist',NULL,'Наименование','4.8','01','Системное меню_сокращ',NULL,'0',NULL),(60,'idalias','sup.syslist',NULL,'ИД_а','2.4..Системное меню_сокращ.Наименование','02','Системное меню_сокращ',NULL,'0',NULL),(61,'idsyslist','sup.syslist','v','ИД','4.19..7','00','Системное меню_полн',NULL,'0',NULL),(62,'alias','sup.syslist',NULL,'Наименование','1.8','01','Системное меню_полн',NULL,'0',NULL),(63,'idalias','sup.syslist',NULL,'ИД_а','2.4..Системное меню_полн.Наименование','02','Системное меню_полн',NULL,'0',NULL),(64,'access','sup.syslist',NULL,'Права доступа','2.9','03','Системное меню_полн',NULL,'0',NULL),(65,'pc','sup.syslist',NULL,'Вызываемая функция','2.2..Методы системного меню_полн.Наименование','04','Системное меню_полн',NULL,'0',NULL),(66,'idmainmenu','sup.mainmenu','v','ИД','4.19..7','00','Главное меню_сокращ',NULL,'0',NULL),(67,'alias','sup.mainmenu',NULL,'Наименование','4.8','01','Главное меню_сокращ',NULL,'0',NULL),(68,'idalias','sup.mainmenu',NULL,'ИД_а','2.4..Главное меню_сокращ.Наименование','02','Главное меню_сокращ',NULL,'0',NULL),(69,'idmainmenu','sup.mainmenu','v','ИД','4.19..7','00','Главное меню_полн','2015-12-16 22:00:44','0','2001'),(70,'alias','sup.mainmenu','','Наименование','1.8','01','Главное меню_полн','2015-12-16 22:00:44','0','2001'),(71,'idalias','sup.mainmenu','','ИД_а','2.4..Главное меню_полн.Наименование','02','Главное меню_полн','2015-12-16 22:00:44','0','2001'),(72,'access','sup.mainmenu','','Права доступа','2.9','03','Главное меню_полн','2015-12-16 22:00:44','0','2001'),(73,'tooltip','sup.mainmenu','','Подсказка в строке статуса','1.8','04','Главное меню_полн','2015-12-16 22:00:44','0','2001'),(74,'method','sup.mainmenu','','Вызываемая функция','2.2..Методы главного меню_сокращ.Наименование','05','Главное меню_полн','2015-12-16 22:00:44','0','2001'),(75,'idmainmenumethods','sup.mainmenumethods','v','ИД','4.19..7','00','Методы главного меню_сокращ',NULL,'0',NULL),(76,'mainmenumethods','sup.mainmenumethods',NULL,'Наименование','1.8','01','Методы главного меню_сокращ',NULL,'0',NULL),(77,'idsysmenumethods','sup.sysmenumethods','v','ИД','4.19..7','00','Методы системного меню_полн',NULL,'0',NULL),(78,'sysmenumethods','sup.sysmenumethods',NULL,'Наименование','1.8','01','Методы системного меню_полн',NULL,'0',NULL),(79,'description','sup.sysmenumethods',NULL,'Описание','1.8','02','Методы системного меню_полн',NULL,'0',NULL),(80,'iddirlist','sup.dirlist','v','ИД','4.19..7','00','Справочники_полн',NULL,'0',NULL),(81,'dirlist','sup.dirlist',NULL,'Наименование','1.8','01','Справочники_полн',NULL,'0',NULL),(82,'access','sup.dirlist',NULL,'Права доступа','2.9','02','Справочники_полн',NULL,'0',NULL),(83,'dirparent','sup.dirlist',NULL,'Родительский справочник','2.2..Справочники_сокращ.Наименование','03','Справочники_полн',NULL,'0',NULL),(84,'iddirsyslist','sup.dirsyslist','v','ИД','4.19..7','00','Справочники системные_сокращ',NULL,'0',NULL),(86,'idmainmenumethods','sup.mainmenumethods','v','ИД','4.19..7','00','Методы главного меню_полн',NULL,'0',NULL),(85,'dirsyslist','sup.dirsyslist',NULL,'Наименование','1.8','01','Справочники системные_сокращ',NULL,'0',NULL),(87,'mainmenumethods','sup.mainmenumethods',NULL,'Наименование','1.8','01','Методы главного меню_полн',NULL,'0',NULL),(88,'description','sup.mainmenumethods',NULL,'Описание','1.8','02','Методы главного меню_полн',NULL,'0',NULL),(89,'iddirsyslist','sup.dirsyslist','v','ИД','4.19..7','00','Справочники системные_полн',NULL,'0',NULL),(90,'dirsyslist','sup.dirsyslist',NULL,'Наименование','1.8','01','Справочники системные_полн',NULL,'0',NULL),(91,'access','sup.dirsyslist',NULL,'Права доступа','2.9','02','Справочники системные_полн',NULL,'0',NULL),(92,'dirparent','sup.dirsyslist',NULL,'Родительский справочник','2.2..Справочники системные_сокращ.Наименование','03','Справочники системные_полн',NULL,'0',NULL),(95,'descriptionfull','alt.description','','Описание','4.8','02','АКомпоненты_описание_полн','2015-11-08 14:26:17','0','2001'),(94,'description','alt.description','','Наименование','4.8','01','АКомпоненты_описание_полн','2015-11-08 14:26:17','0','2001'),(93,'iddescription','alt.description','v','ИД','4.19..7','00','АКомпоненты_описание_полн','2015-11-08 14:26:17','0','2001'),(96,'idsysmenumethods','sup.sysmenumethods','v','ИД','4.19..7','00','Методы системного меню_сокращ',NULL,'0',NULL),(97,'sysmenumethods','sup.sysmenumethods',NULL,'Наименование','1.8','01','Методы системного меню_сокращ',NULL,'0',NULL),(98,'idunitset','ent.unitset','v','ИД','4.19..7','00','Единицы измерения_сокращ','09/08/2015 14:00:23','0','2001'),(99,'idparameters','ent.parameters','v','ИД','4.19..7','00','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(100,'alias','ent.unitset','','Наименование','1.8..','01','Единицы измерения_сокращ','09/08/2015 14:00:23','0','2001'),(101,'idalias','ent.unitset','','ИД_а','2.4..Единицы измерения_сокращ.Наименование','02','Единицы измерения_сокращ','09/08/2015 14:00:23','0','2001'),(102,'parameters','ent.parameters','','Наименование','1.8..','01','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(103,'par1name','ent.parameters','','Параметр1','1.8..','02','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(104,'par2name','ent.parameters','','Параметр2','1.8..','03','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(105,'par3name','ent.parameters','','Параметр3','1.8..','04','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(106,'par4name','ent.parameters','','Параметр4','1.8..','05','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(107,'par5name','ent.parameters','','Параметр5','1.8..','06','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(108,'par1unitset','ent.parameters','','Ед. изм. 1','2.2..Единицы измерения_сокращ.Наименование','07','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(109,'par2unitset','ent.parameters','','Ед. изм. 2','2.2..Единицы измерения_сокращ.Наименование','08','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(110,'par3unitset','ent.parameters','','Ед. изм. 3','2.2..Единицы измерения_сокращ.Наименование','09','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(111,'par4unitset','ent.parameters','','Ед. изм. 4','2.2..Единицы измерения_сокращ.Наименование','10','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(112,'par5unitset','ent.parameters','','Ед. изм. 5','2.2..Единицы измерения_сокращ.Наименование','11','Наборы параметров_полн','30/08/2015 17:07:22','0','2001'),(113,'idunitset','ent.unitset','v','ИД','4.19..7','00','Единицы измерения_полн','15/08/2015 19:00:07','0','2001'),(114,'alias','ent.unitset','','Наименование','1.8..','01','Единицы измерения_полн','15/08/2015 19:00:07','0','2001'),(115,'idalias','ent.unitset','','ИД_а','2.4..Единицы измерения_сокращ.Наименование','02','Единицы измерения_полн','15/08/2015 19:00:07','0','2001'),(116,'idmanuf','ent.manuf','v','ИД','4.19..7','00','Производители_полн','2015-12-25 22:07:14','0','2001'),(117,'manuf','ent.manuf','','Наименование','1.8..','01','Производители_полн','2015-12-25 22:07:14','0','2001'),(118,'www','ent.manuf','','Адрес в интернете','1.8..','02','Производители_полн','2015-12-25 22:07:14','0','2001'),(126,'access','sup.groups','','Доступ','2.9..','02','Группы доступа_полн','27/08/2015 20:00:21','0','2001'),(125,'groups','sup.groups','','Группа доступа','1.8..','01','Группы доступа_полн','27/08/2015 20:00:21','0','2001'),(124,'idgroups','sup.groups','v','ИД','4.19..7','00','Группы доступа_полн','27/08/2015 20:00:21','0','2001'),(123,'group','sup.personel','','Группа доступа','2.9..','04','Персонал_полн','2015-09-24 21:37:43','0','2001'),(121,'login','sup.personel','','Имя','1.8..','02','Персонал_полн','2015-09-24 21:37:43','0','2001'),(122,'psw','sup.personel','','Пароль','1.8..','03','Персонал_полн','2015-09-24 21:37:43','0','2001'),(120,'personel','sup.personel','','ФИО','1.8..','01','Персонал_полн','2015-09-24 21:37:43','0','2001'),(119,'idpersonel','sup.personel','v','ИД','4.19..7','00','Персонал_полн','2015-09-24 21:37:43','0','2001'),(127,'idgroups','sup.groups','v','ИД','4.19..7','00','Группы доступа_сокращ','27/08/2015 20:00:21','0','2001'),(128,'groups','sup.groups','','Группа доступа','1.8..','01','Группы доступа_сокращ','27/08/2015 20:00:21','0','2001'),(129,'access','sup.groups','','Доступ','2.9..','02','Группы доступа_сокращ','27/08/2015 20:00:21','1','2001'),(130,'idpersonel','sup.personel','v','ИД','4.19..7','00','Персонал_сокращ','27/08/2015 20:07:06','0','2001'),(131,'personel','sup.personel','','ФИО','1.8..','01','Персонал_сокращ','27/08/2015 20:07:06','0','2001'),(132,'group','sup.personel','','Группа доступа','3.2..АКомпоненты_описание_полн.Наименование','04','Персонал_сокращ','27/08/2015 20:07:06','1','2001'),(133,'idcontractors','ent.contractors','v','ИД','4.19..7','00','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(134,'contractors','ent.contractors','','Наименование','1.8..','01','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(135,'country','ent.contractors','','Страна','1.8..','02','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(136,'index','ent.contractors','','Почтовый индекс','1.6..\"^[0-9]{6}$\"','03','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(137,'city','ent.contractors','','Город','1.8..','04','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(138,'address','ent.contractors','','Почтовый адрес','1.8..','05','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(139,'phone','ent.contractors','','Телефон','1.6..\"^[0-9]{1,15}$\"','06','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(140,'manager','ent.contractors','','Отв. лицо','1.8..','07','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(141,'phonemgr','ent.contractors','','Телефон отв. лица','1.8..','08','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(142,'inn','ent.contractors','','ИНН','1.6..\"^[0-9]{1,15}$\"','09','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(143,'kpp','ent.contractors','','КПП','1.6..\"^[0-9]{1,15}$\"','10','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(144,'benefacc','ent.contractors','','Расч. счёт','1.6..\"^[0-9]{20}$\"','11','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(145,'benefbank','ent.contractors','','Банк','1.8..','12','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(146,'bik','ent.contractors','','БИК','1.8..','13','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(147,'corracc','ent.contractors','','Корр. счёт','1.6..\"^[0-9]{20}$\"','14','Контрагенты_полн','27/08/2015 21:43:55','0','2001'),(148,'idcontractors','ent.contractors','v','ИД','4.19..7','00','Контрагенты_сокращ','27/08/2015 21:43:55','0','2001'),(149,'contractors','ent.contractors','','Наименование','1.8..','01','Контрагенты_сокращ','27/08/2015 21:43:55','0','2001'),(150,'country','ent.contractors','','Страна','1.8..','02','Контрагенты_сокращ','27/08/2015 21:43:55','0','2001'),(40,'iddocclasses','ent.docclasses','v','ИД','4.19..7','00','Классы документов_сокращ','2015-12-06 20:44:57','0','2001'),(41,'docclasses','ent.docclasses','','Наименование','1.8..','01','Классы документов_сокращ','2015-12-06 20:44:57','0','2001'),(39,'iddocclasses','ent.docclasses','v','ИД','4.19..7','00','Классы документов_полн','2015-12-06 20:44:57','0','2001'),(38,'docclasses','ent.docclasses','','Наименование','1.8..','01','Классы документов_полн','2015-12-06 20:44:57','0','2001'),(158,'idalias','ent.whplaces','','ИД_а','2.4..Склады размещение_полн.ИД','02','Склады размещение_полн','2015-10-09 15:48:16','0','2001'),(157,'alias','ent.whplaces','','Наименование','1.8..','01','Склады размещение_полн','2015-10-09 15:48:16','0','2001'),(159,'idwhplaces','ent.whplaces','v','ИД','4.19..7','00','Склады размещение_полн','2015-10-09 15:48:16','0','2001'),(160,'idwhplaces','ent.whplaces','v','ИД','4.19..7','00','Склады размещение_сокращ','2015-10-09 15:48:16','0','2001'),(161,'alias','ent.whplaces','','Наименование','1.8..','01','Склады размещение_сокращ','2015-10-09 15:48:16','0','2001'),(162,'idalias','ent.whplaces','','ИД_а','2.4..Склады размещение_полн.ИД','02','Склады размещение_сокращ','2015-10-09 15:48:16','0','2001'),(163,'description','ent.whplaces','','Описание','1.8..','03','Склады размещение_полн','2015-10-09 15:48:16','0','2001'),(174,'whnum','ent.whplaces','','Номер','5.10..n','06','Склады размещение_полн','2015-10-09 15:48:16','0','2001'),(177,'whplacetanks','ent.whplacetanks','','Наименование','1.8..','01','Склады ёмкости размещения_сокращ','2015-10-04 14:41:37','0','2001'),(166,'rows','ent.whplacetypes','','Кол-во этажей','5.10..n','03','Склады типы размещения_полн','2015-10-05 21:50:23','0','2001'),(167,'idwhplacetypes','ent.whplacetypes','v','ИД','4.19..7','00','Склады типы размещения_полн','2015-10-05 21:50:23','0','2001'),(168,'idwhplacetypes','ent.whplacetypes','v','ИД','4.19..7','00','Склады типы размещения_сокращ','2015-10-05 21:50:23','0','2001'),(169,'columns','ent.whplacetypes','','Кол-во рядов','5.10..n','02','Склады типы размещения_полн','2015-10-05 21:50:23','0','2001'),(165,'type','ent.whplacetypes','','Тип размещения','2.2..Склады ёмкости размещения_сокращ.Наименование','04','Склады типы размещения_полн','2015-10-05 21:50:23','0','2001'),(164,'wh','ent.whplaces','','Склад','2.2..Склады_сокращ.Наименование','05','Склады размещение_полн','2015-10-09 15:48:16','0','2001'),(173,'name','ent.whplaces','','Обозначение','1.8..','04','Склады размещение_полн','2015-10-09 15:48:16','0','2001'),(175,'placetype','ent.whplaces','','Тип размещения','2.2..Склады типы размещения_сокращ.Наименование','07','Склады размещение_полн','2015-10-09 15:48:16','0','2001'),(172,'wh','ent.whplacetypes','','Принадлежность складу','2.2..Склады_сокращ.Наименование','05','Склады типы размещения_сокращ','2015-10-05 21:49:59','1','2001'),(180,'picture','ent.whplacetanks','','Картинка','1.8..','02','Склады ёмкости размещения_полн','2015-10-04 14:41:37','0','2001'),(171,'priority','ent.whplacetanks','','Приоритет вложенности','5.10..n','03','Склады ёмкости размещения_полн','2015-10-04 22:05:01','0','2001'),(178,'whplacetypes','ent.whplacetypes','','Наименование','1.8..','01','Склады типы размещения_полн','2015-10-05 21:50:23','0','2001'),(179,'whplacetypes','ent.whplacetypes','','Наименование','1.8..','01','Склады типы размещения_сокращ','2015-10-05 21:50:23','0','2001'),(170,'whplacetanks','ent.whplacetanks','','Наименование','1.8..','01','Склады ёмкости размещения_полн','2015-10-04 14:41:37','0','2001'),(181,'idwhplacetanks','ent.whplacetanks','v','ИД','4.19..7','00','Склады ёмкости размещения_полн','2015-10-04 14:41:37','0','2001'),(182,'idwhplacetanks','ent.whplacetanks','v','ИД','4.19..7','00','Склады ёмкости размещения_сокращ','2015-10-04 14:41:37','0','2001'),(153,'order','sup.mainmenu','','Приоритет','5.10..nn','06','Главное меню_полн','2015-12-16 22:00:44','0','2001'),(154,'url','alt.description','','Путь к файлам','2.16..','03','АКомпоненты_описание_полн','2015-11-08 14:26:17','0','2001'),(156,'iddevices','dev.lists','','Изделие','','01','Перечни_полн','2015-11-08 15:51:56','0','2001'),(176,'idlists','dev.lists','v','ИД','4.19..7','00','Перечни_полн','2015-11-08 15:51:56','0','2001'),(183,'idlists','dev.lists','v','ИД','4.19..7','00','Перечни_сокращ','2015-11-08 15:51:56','0','2001'),(184,'devices','dev.devices','','Наименование','1.8..','02','Изделия_полн','2015-12-25 21:02:47','0','2001'),(185,'devices','dev.devices','','Наименование','1.8..','02','Изделия_сокращ','2015-12-25 21:02:47','0','2001'),(186,'decimal','dev.devices','','Децимальный номер','1.8..','01','Изделия_полн','2015-12-25 21:02:47','0','2001'),(187,'decimal','dev.devices','','Децимальный номер','1.8..','01','Изделия_сокращ','2015-12-25 21:02:47','0','2001'),(188,'idmanuf','dev.devices','','Производитель','2.2..Производители_сокращ.Наименование','03','Изделия_полн','2015-12-25 21:02:47','0','2001'),(189,'idmanuf','dev.devices','','Производитель','2.2..Производители_сокращ.Наименование','03','Изделия_сокращ','2015-12-25 21:02:47','0','2001'),(190,'description','dev.devices','','Описание для перечня','1.8..','04','Изделия_полн','2015-12-25 21:02:47','0','2001'),(191,'notes','dev.devices','','Примечание','1.8..','05','Изделия_полн','2015-12-25 21:02:47','0','2001'),(192,'url','dev.devices','','Путь к каталогу','2.16..','06','Изделия_полн','2015-12-25 21:02:47','0','2001'),(193,'isobsolete','dev.devices','','Снято с производства','1.8..','07','Изделия_полн','2015-12-25 21:02:47','0','2001'),(194,'iddevices','dev.devices','v','ИД','4.19..7','00','Изделия_полн','2015-12-25 21:02:47','0','2001'),(195,'iddevices','dev.devices','v','ИД','4.19..7','00','Изделия_сокращ','2015-12-25 21:02:47','0','2001'),(196,'first','dev.devices','','В составе (перв. прим.)','1.8..','08','Изделия_полн','2015-12-25 21:02:47','0','2001'),(197,'creator','dev.devices','','Разработчик','2.2..Персонал_сокращ.ФИО','09','Изделия_полн','2015-12-25 21:02:47','0','2001'),(152,'url','ent.documents','','Ссылка на документ','2.15..','03','Документы_полн','2015-12-06 22:32:54','0','2001'),(198,'pic','ent.documents','','Файл картинки','2.15..','04','Документы_полн','2015-12-06 22:32:54','0','2001'),(199,'type','ent.documents','','Тип документа','1.8..','05','Документы_полн','2015-12-06 22:32:54','0','2001'),(200,'md5sum','ent.documents','','Контрольная сумма','1.8..','06','Документы_полн','2015-12-06 22:32:54','0','2001'),(201,'signed','ent.documents','','Подписан','1.8..','07','Документы_полн','2015-12-06 22:32:54','0','2001'),(202,'idsignpers','ent.documents','','Подписал','2.2..Персонал_сокращ.ФИО','08','Документы_полн','2015-12-06 22:32:54','0','2001'),(203,'iddocuments','ent.documents','v','ИД','4.19..7','00','Документы_полн','2015-12-06 22:32:54','0','2001'),(204,'iddocuments','ent.documents','v','ИД','4.19..7','00','Документы_сокращ','2015-12-06 22:32:54','0','2001'),(205,'documents','ent.documents','','Наименование','1.8..','01','Документы_сокращ','2015-12-06 22:32:54','0','2001'),(206,'docclasses','ent.documents','','Класс документа','2.2..Классы документов_сокращ.Наименование','02','Документы_сокращ','2015-12-06 22:32:54','0','2001'),(207,'documents','dev.documents','','Документ','2.2..Документы_сокращ.Наименование','01','Документация_полн','2015-12-06 20:58:51','0','2001'),(208,'documents','dev.documents','','Документ','2.2..Документы_сокращ.Наименование','01','Документация_сокращ','2015-12-06 20:58:51','0','2001'),(209,'device','dev.documents','','Изделие','2.2..Изделия_сокращ.Наименование','02','Документация_полн','2015-12-06 20:58:51','0','2001'),(210,'device','dev.documents','','Изделие','2.2..Изделия_сокращ.Наименование','02','Документация_сокращ','2015-12-06 20:58:51','0','2001'),(211,'iddocuments','dev.documents','v','ИД','4.19..7','00','Документация_полн','2015-12-06 20:58:51','0','2001'),(212,'iddocuments','dev.documents','v','ИД','4.19..7','00','Документация_сокращ','2015-12-06 20:58:51','0','2001'),(155,'idmainmenu','sup.mainmenu','v','ИД','4.19..7','00','','2015-12-16 22:00:44','0','2001'),(213,'decimal','ent.manuf','','Код предприятия','1.6..\"^\\D{4}$\"','03','Производители_полн','2015-12-25 22:07:14','0','2001'),(214,'alias','ent.eskd_class','','Наименование','1.8..','03','Классификатор ЕСКД_полн','2015-12-27 13:12:50','0','2001'),(215,'alias','ent.eskd_class','','Наименование','1.8..','03','Классификатор ЕСКД_сокращ','2015-12-27 13:12:50','0','2001'),(216,'idalias','ent.eskd_class','','ИД_а','2.4..Классификатор ЕСКД_полн.Наименование','02','Классификатор ЕСКД_полн','2015-12-27 13:12:50','0','2001'),(217,'class','ent.eskd_class','','Класс','1.8..','01','Классификатор ЕСКД_полн','2015-12-27 13:12:50','0','2001'),(218,'ideskd_class','ent.eskd_class','v','ИД','4.19..7','00','Классификатор ЕСКД_сокращ','2015-12-27 13:12:50','0','2001'),(219,'class','ent.eskd_class','','Класс','1.8..','01','Классификатор ЕСКД_сокращ','2015-12-27 13:12:50','0','2001'),(220,'ideskd_class','ent.eskd_class','v','ИД','4.19..7','00','Классификатор ЕСКД_полн','2015-12-27 13:12:50','0','2001'),(221,'idalias','ent.eskd_class','','ИД_а','2.4..Классификатор ЕСКД_полн.Наименование','02','Классификатор ЕСКД_сокращ','2015-12-27 13:12:50','0','2001');
/*!40000 ALTER TABLE `tablefields` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-12-27 14:11:43
