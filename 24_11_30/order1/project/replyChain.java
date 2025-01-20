// 用于管理和打印评论链的类

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class replyChain {
    public static void checkchain(String name) {
        try {
            List<String> lines = readFileToList(".\\commnets.txt");
            List<comment> comments = parseComments(lines);
            Map<Integer, comment> commentMap = buildCommentMap(comments);
            List<comment> rootComments = findRootComments(comments);

            for (comment root : rootComments) {
                printCommentChain(name, root, commentMap, "");
            }
        } catch (IOException e) {
            System.err.println("读取文件时发生错误: " + e.getMessage());
        }
    }

    public static List<String> readFileToList(String filePath) throws IOException {
        return Files.readAllLines(Paths.get(filePath));
    }

    private static List<comment> parseComments(List<String> lines) {
        List<comment> comments = new ArrayList<>();
        for (String line : lines) {
            String[] parts = line.split(",");
            int id = Integer.parseInt(parts[0]);
            String title = parts[1];
            String author = parts[2];
            String content = parts[3];
            int parentId = Integer.parseInt(parts[4]);
            comments.add(new comment(id, title, author, content, parentId));
        }
        return comments;
    }

    private static Map<Integer, comment> buildCommentMap(List<comment> comments) {
        Map<Integer, comment> commentMap = new HashMap<>();
        for (comment c : comments) {
            commentMap.put(c.getId(), c);
        }
        return commentMap;
    }

    private static List<comment> findRootComments(List<comment> comments) {
        List<comment> rootComments = new ArrayList<>();
        for (comment c : comments) {
            if (c.getParentId() == -1) {
                rootComments.add(c);
            }
        }
        return rootComments;
    }

    private static void printCommentChain(String name, comment comment, Map<Integer, comment> commentMap,
            String indent) {
        if (name.equals(comment.getAuthor())) {
            System.out.println(indent + "ID: " + comment.getId() + ", Title: " + comment.getTitle() + ", Author: "
                    + comment.getAuthor() + ", Content: " + comment.getContent());
            for (comment child : commentMap.values()) {
                if (child.getParentId() == comment.getId()) {
                    printCommentChain(name, child, commentMap, indent + "  ");
                }
            }
        }
    }
}