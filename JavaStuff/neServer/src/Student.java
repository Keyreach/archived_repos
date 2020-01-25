/**
 * Created by ake on 13.10.14.
 */
public class Student {
    private String Name;
    private String Group;
    public void setName(String _Name){
        this.Name = _Name;
    }
    public void setGroup(String _Group){
        this.Group = _Group;
    }
    public String getName(){
        return this.Name;
    }
    public String getGroup(){
        return this.Group;
    }
    public Student(String _Name, String _Group){
        this.Name = _Name;
        this.Group = _Group;

    }
}
