import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class fileUtile {

    // public static void main(String[] args) {
    // Map<String, Integer> linkedMap = new LinkedHashMap<>();
    // linkedMap.put("apple", 10);
    // linkedMap.put("banana", 5);
    // linkedMap.put("orange", 8);

    // down("out.txt", linkedMap);
    // }

    public static void down(String path, Map<String, String> data) {

        try {
            List<String> lines = new ArrayList<>();
            for (Map.Entry<String, String> entry : data.entrySet()) {
                lines.add(entry.getKey() + ":" + entry.getValue());
            }
            Files.write(Paths.get(path), lines, StandardCharsets.UTF_8);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static List<String> listFileNames(String directoryPath) {
        List<String> fileNames = new ArrayList<>();

        try (Stream<Path> paths = Files.walk(Paths.get(directoryPath))) {
            fileNames = paths.filter(Files::isRegularFile)
                    .map(Path::getFileName)
                    .map(p -> p.toString().split("\\.")[0])
                    .collect(Collectors.toList());
        } catch (IOException e) {
            e.printStackTrace();
        }

        return fileNames;
    }

    public static void deleteFile(String filePath) {
        Path path = Paths.get(filePath);

        // 检查文件是否存在
        if (!Files.exists(path)) {
            System.err.println("文件不存在: " + filePath);
            return;
        }

        try {
            Files.delete(path);
            System.out.println("文件删除成功: " + filePath);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static String readFileToString(String filePath) throws IOException {
        Path path = Paths.get(filePath);
        // 使用Files.readString方法读取文件内容
        return Files.readString(path, StandardCharsets.UTF_8).toString();
    }

    // public static void downComments(String path, ArrayList<comment> comments) {
    // // 实现保存评论的逻辑
    // // 例如，将评论列表写入文件
    // List<String> lines = new ArrayList<>();
    // for (comment c : comments) {
    // String line = c.getId() + "," + c.getTitle() + "," + c.getAuthor() + ","
    // + c.getContent() + "\n";
    // lines.add(line);
    // }
    // try {
    // Files.write(Paths.get(path), lines, StandardCharsets.UTF_8,
    // StandardOpenOption.APPEND);
    // } catch (IOException e) {
    // e.printStackTrace();
    // }
    // }

    public static Integer commentsSize(String path) {
        try {
            List<String> lines = Files.readAllLines(Paths.get(path),
                    StandardCharsets.UTF_8);
            int length = lines.size();
            return length;

        } catch (IOException e) {
            e.printStackTrace();
            return -1;
        }

    }

    public static void downComments(String filePath, List<comment> comments) throws IOException {
        StringBuilder sb = new StringBuilder();
        for (comment c : comments) {
            sb.append(c.getId()).append(",")
                    .append(c.getTitle()).append(",")
                    .append(c.getAuthor()).append(",")
                    .append(c.getContent()).append(",")
                    .append(c.getParentId()).append("\n");
        }
        Files.write(Paths.get(filePath), sb.toString().getBytes(), StandardOpenOption.APPEND);
    }

    // public static int commentsSize(String filePath) throws IOException {
    // List<String> lines = Files.readAllLines(Paths.get(filePath));
    // return lines.size();
    // }
}
