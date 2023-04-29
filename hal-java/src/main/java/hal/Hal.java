package hal;

public class Hal {

    private Hal() {}

    public static HalEnv initialize() {
        long ptr = HALJNI.init();
        return new HalEnv(ptr);
    }
}
