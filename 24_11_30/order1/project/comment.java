// 评论类

public class comment {
    private int id;
    private String title;
    private String author;
    private String content;
    private int parentId; // 新增字段，用于存储回复的评论 ID

    // 构造方法
    public comment() {
    }

    public comment(int id, String title, String author, String content, int parentId) {
        this.id = id;
        this.title = title;
        this.author = author;
        this.content = content;
        this.parentId = parentId;
    }

    // Getter 和 Setter 方法
    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getContent() {
        return content;
    }

    public void setContent(String content) {
        this.content = content;
    }

    public int getParentId() {
        return parentId;
    }

    public void setParentId(int parentId) {
        this.parentId = parentId;
    }
}