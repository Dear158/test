/*
 Navicat MySQL Data Transfer

 Source Server         : MySQL
 Source Server Type    : MySQL
 Source Server Version : 80013 (8.0.13)
 Source Host           : localhost:3306
 Source Schema         : gameproject

 Target Server Type    : MySQL
 Target Server Version : 80013 (8.0.13)
 File Encoding         : 65001

 Date: 24/06/2024 22:57:43
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for currency
-- ----------------------------
DROP TABLE IF EXISTS `currency`;
CREATE TABLE `currency`  (
  `user_id` int(11) NOT NULL,
  `beans` int(11) NOT NULL DEFAULT 0,
  INDEX `user_id`(`user_id` ASC) USING BTREE,
  CONSTRAINT `currency_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of currency
-- ----------------------------
INSERT INTO `currency` VALUES (39, 0);
INSERT INTO `currency` VALUES (1, 100);
INSERT INTO `currency` VALUES (8, 200);
INSERT INTO `currency` VALUES (12, 0);
INSERT INTO `currency` VALUES (13, 0);
INSERT INTO `currency` VALUES (29, 0);
INSERT INTO `currency` VALUES (40, 0);
INSERT INTO `currency` VALUES (41, 0);
INSERT INTO `currency` VALUES (42, 0);
INSERT INTO `currency` VALUES (43, 0);
INSERT INTO `currency` VALUES (44, 0);
INSERT INTO `currency` VALUES (45, 500);
INSERT INTO `currency` VALUES (46, 0);

-- ----------------------------
-- Table structure for friends
-- ----------------------------
DROP TABLE IF EXISTS `friends`;
CREATE TABLE `friends`  (
  `user_id` int(11) NOT NULL,
  `friend_id` int(11) NOT NULL,
  `status` enum('pending','accepted') CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  INDEX `user_id`(`user_id` ASC) USING BTREE,
  INDEX `friend_id`(`friend_id` ASC) USING BTREE,
  CONSTRAINT `friends_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `friends_ibfk_2` FOREIGN KEY (`friend_id`) REFERENCES `users` (`user_id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of friends
-- ----------------------------
INSERT INTO `friends` VALUES (8, 13, 'accepted');
INSERT INTO `friends` VALUES (45, 8, 'accepted');
INSERT INTO `friends` VALUES (45, 1, 'accepted');
INSERT INTO `friends` VALUES (8, 1, 'accepted');
INSERT INTO `friends` VALUES (46, 45, 'pending');

-- ----------------------------
-- Table structure for matches
-- ----------------------------
DROP TABLE IF EXISTS `matches`;
CREATE TABLE `matches`  (
  `match_id` int(11) NOT NULL AUTO_INCREMENT,
  `start_time` datetime NOT NULL,
  `end_time` datetime NULL DEFAULT NULL,
  `num` int(11) NULL DEFAULT 1,
  `status` enum('waiting','ongoing','finished') CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  PRIMARY KEY (`match_id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 128 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of matches
-- ----------------------------
INSERT INTO `matches` VALUES (112, '2024-06-24 15:08:16', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (113, '2024-06-24 21:19:54', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (114, '2024-06-24 21:23:29', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (115, '2024-06-24 21:28:12', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (116, '2024-06-24 21:31:32', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (117, '2024-06-24 21:35:47', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (118, '2024-06-24 21:39:18', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (119, '2024-06-24 21:41:35', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (120, '2024-06-24 21:42:25', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (121, '2024-06-24 21:53:41', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (122, '2024-06-24 22:02:41', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (123, '2024-06-24 22:03:50', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (124, '2024-06-24 22:06:26', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (125, '2024-06-24 22:34:03', NULL, 3, 'ongoing');
INSERT INTO `matches` VALUES (126, '2024-06-24 22:39:11', NULL, 3, 'ongoing');

-- ----------------------------
-- Table structure for user_matches
-- ----------------------------
DROP TABLE IF EXISTS `user_matches`;
CREATE TABLE `user_matches`  (
  `user_id` int(11) NOT NULL,
  `match_id` int(11) NOT NULL,
  INDEX `user_id`(`user_id` ASC) USING BTREE,
  INDEX `match_id`(`match_id` ASC) USING BTREE,
  CONSTRAINT `user_matches_ibfk_1` FOREIGN KEY (`user_id`) REFERENCES `users` (`user_id`) ON DELETE RESTRICT ON UPDATE RESTRICT,
  CONSTRAINT `user_matches_ibfk_2` FOREIGN KEY (`match_id`) REFERENCES `matches` (`match_id`) ON DELETE RESTRICT ON UPDATE RESTRICT
) ENGINE = InnoDB CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of user_matches
-- ----------------------------
INSERT INTO `user_matches` VALUES (1, 112);
INSERT INTO `user_matches` VALUES (8, 112);
INSERT INTO `user_matches` VALUES (12, 112);
INSERT INTO `user_matches` VALUES (1, 113);
INSERT INTO `user_matches` VALUES (8, 113);
INSERT INTO `user_matches` VALUES (12, 113);
INSERT INTO `user_matches` VALUES (1, 114);
INSERT INTO `user_matches` VALUES (8, 114);
INSERT INTO `user_matches` VALUES (12, 114);
INSERT INTO `user_matches` VALUES (1, 115);
INSERT INTO `user_matches` VALUES (8, 115);
INSERT INTO `user_matches` VALUES (12, 115);
INSERT INTO `user_matches` VALUES (1, 116);
INSERT INTO `user_matches` VALUES (8, 116);
INSERT INTO `user_matches` VALUES (12, 116);
INSERT INTO `user_matches` VALUES (1, 117);
INSERT INTO `user_matches` VALUES (8, 117);
INSERT INTO `user_matches` VALUES (12, 117);
INSERT INTO `user_matches` VALUES (1, 118);
INSERT INTO `user_matches` VALUES (8, 118);
INSERT INTO `user_matches` VALUES (12, 118);
INSERT INTO `user_matches` VALUES (1, 119);
INSERT INTO `user_matches` VALUES (8, 119);
INSERT INTO `user_matches` VALUES (12, 119);
INSERT INTO `user_matches` VALUES (1, 120);
INSERT INTO `user_matches` VALUES (8, 120);
INSERT INTO `user_matches` VALUES (12, 120);
INSERT INTO `user_matches` VALUES (1, 121);
INSERT INTO `user_matches` VALUES (8, 121);
INSERT INTO `user_matches` VALUES (12, 121);
INSERT INTO `user_matches` VALUES (1, 122);
INSERT INTO `user_matches` VALUES (8, 122);
INSERT INTO `user_matches` VALUES (12, 122);
INSERT INTO `user_matches` VALUES (1, 123);
INSERT INTO `user_matches` VALUES (8, 123);
INSERT INTO `user_matches` VALUES (12, 123);
INSERT INTO `user_matches` VALUES (1, 124);
INSERT INTO `user_matches` VALUES (8, 124);
INSERT INTO `user_matches` VALUES (12, 124);
INSERT INTO `user_matches` VALUES (1, 125);
INSERT INTO `user_matches` VALUES (8, 125);
INSERT INTO `user_matches` VALUES (12, 125);
INSERT INTO `user_matches` VALUES (1, 126);
INSERT INTO `user_matches` VALUES (8, 126);
INSERT INTO `user_matches` VALUES (12, 126);

-- ----------------------------
-- Table structure for users
-- ----------------------------
DROP TABLE IF EXISTS `users`;
CREATE TABLE `users`  (
  `user_id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `password` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NOT NULL,
  `email` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT NULL,
  `is_active` tinyint(1) NOT NULL DEFAULT 1,
  `last_login` datetime NULL DEFAULT NULL,
  PRIMARY KEY (`user_id`) USING BTREE,
  UNIQUE INDEX `username`(`username` ASC) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 47 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of users
-- ----------------------------
INSERT INTO `users` VALUES (1, '1', '1', '123', 0, NULL);
INSERT INTO `users` VALUES (8, '2', '2', '12', 1, NULL);
INSERT INTO `users` VALUES (12, '3', '3', '88', 1, NULL);
INSERT INTO `users` VALUES (13, '5', '5', NULL, 0, NULL);
INSERT INTO `users` VALUES (29, 'e', 'e', 'e', 0, NULL);
INSERT INTO `users` VALUES (39, 'q', 'q', NULL, 0, NULL);
INSERT INTO `users` VALUES (40, 'w', 'w', NULL, 0, NULL);
INSERT INTO `users` VALUES (41, 't', 't', NULL, 0, NULL);
INSERT INTO `users` VALUES (42, 'y', 'y', NULL, 0, NULL);
INSERT INTO `users` VALUES (43, 'i', 'i', NULL, 0, NULL);
INSERT INTO `users` VALUES (44, 'o', 'o', NULL, 0, NULL);
INSERT INTO `users` VALUES (45, 'wdy', 'wdy', 'wdy@163.com', 0, NULL);
INSERT INTO `users` VALUES (46, 'zhm', 'zhm', '2222', 0, NULL);

SET FOREIGN_KEY_CHECKS = 1;
