-- MySQL dump 10.13  Distrib 5.7.22, for Linux (x86_64)
--
-- Host: localhost    Database: utalk
-- ------------------------------------------------------
-- Server version	5.7.22-0ubuntu0.16.04.1

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
-- Table structure for table `allgroup`
--

DROP TABLE IF EXISTS `allgroup`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `allgroup` (
  `groupname` char(20) NOT NULL,
  PRIMARY KEY (`groupname`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `allgroup`
--

LOCK TABLES `allgroup` WRITE;
/*!40000 ALTER TABLE `allgroup` DISABLE KEYS */;
INSERT INTO `allgroup` VALUES ('g1'),('g2'),('g3');
/*!40000 ALTER TABLE `allgroup` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `alluser`
--

DROP TABLE IF EXISTS `alluser`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alluser` (
  `username` char(20) NOT NULL,
  `password` char(20) DEFAULT NULL,
  PRIMARY KEY (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `alluser`
--

LOCK TABLES `alluser` WRITE;
/*!40000 ALTER TABLE `alluser` DISABLE KEYS */;
INSERT INTO `alluser` VALUES ('123','123'),('dada','123456'),('fhixc','123456'),('fihxc','123456'),('test','123456');
/*!40000 ALTER TABLE `alluser` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `avator`
--

DROP TABLE IF EXISTS `avator`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `avator` (
  `username` char(20) DEFAULT NULL,
  `number` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `avator`
--

LOCK TABLES `avator` WRITE;
/*!40000 ALTER TABLE `avator` DISABLE KEYS */;
INSERT INTO `avator` VALUES ('dada',1),('fhixc',0),('fihxc',3),('test',0),('123',0);
/*!40000 ALTER TABLE `avator` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `friend_list`
--

DROP TABLE IF EXISTS `friend_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `friend_list` (
  `p1` char(20) DEFAULT NULL,
  `p2` char(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `friend_list`
--

LOCK TABLES `friend_list` WRITE;
/*!40000 ALTER TABLE `friend_list` DISABLE KEYS */;
INSERT INTO `friend_list` VALUES ('dada','fihxc'),('fihxc','dada'),('dada','test'),('test','dada'),('fihxc','test'),('test','fihxc'),('dada','123'),('123','dada'),('fihxc','23'),('23','fihxc'),('test','23'),('23','test');
/*!40000 ALTER TABLE `friend_list` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `group_avator`
--

DROP TABLE IF EXISTS `group_avator`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_avator` (
  `groupname` char(20) NOT NULL,
  `number` int(11) DEFAULT NULL,
  PRIMARY KEY (`groupname`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_avator`
--

LOCK TABLES `group_avator` WRITE;
/*!40000 ALTER TABLE `group_avator` DISABLE KEYS */;
INSERT INTO `group_avator` VALUES ('g1',0);
/*!40000 ALTER TABLE `group_avator` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `group_list`
--

DROP TABLE IF EXISTS `group_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `group_list` (
  `p1` char(20) NOT NULL,
  `p2` char(20) DEFAULT NULL,
  PRIMARY KEY (`p1`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `group_list`
--

LOCK TABLES `group_list` WRITE;
/*!40000 ALTER TABLE `group_list` DISABLE KEYS */;
INSERT INTO `group_list` VALUES ('dada','g1'),('fihxc','g1'),('test','g1');
/*!40000 ALTER TABLE `group_list` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-05-20 12:24:20
