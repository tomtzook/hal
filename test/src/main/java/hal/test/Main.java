package hal.test;

import com.castle.code.NativeLibrary;
import com.castle.code.NativeLibraryFinder;
import com.castle.code.loader.NativeLibraryLoader;
import com.castle.code.loader.TempNativeLibraryLoader;
import com.castle.nio.temp.TempPathGenerator;
import com.castle.nio.zip.ArchivedNativeLibraryFinder;
import com.castle.nio.zip.Zip;
import com.castle.util.java.JavaSources;
import com.hal.Hal;
import com.hal.PortDirection;

public class Main {

    public static void main(String[] args) throws Exception {
        loadNative();

        try (Hal hal = Hal.initialize()) {
            int handle = hal.dioInit(0, PortDirection.OUTPUT);
            try {
                hal.dioSet(handle, true);
            } finally {
                hal.dioFree(handle);
            }
        }
    }

    private static void loadNative() throws Exception {
        Zip zip = JavaSources.currentJar(Hal.class);

        NativeLibraryLoader loader = new TempNativeLibraryLoader(new TempPathGenerator());
        NativeLibraryFinder finder = new ArchivedNativeLibraryFinder(zip);

        NativeLibrary nativeLibrary = finder.find("libhaljni");
        loader.load(nativeLibrary);
    }
}
