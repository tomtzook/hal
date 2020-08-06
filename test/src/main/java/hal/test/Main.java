package hal.test;

import com.castle.code.NativeLibrary;
import com.castle.code.finder.NativeLibraryFinder;
import com.castle.code.loader.NativeLibraryLoader;
import com.castle.code.loader.TempNativeLibraryLoader;
import com.castle.nio.zip.ArchivedNativeLibraryFinder;
import com.castle.nio.zip.Zip;
import com.castle.util.java.JavaSources;
import com.flash3388.flashlib.hal.HalIoChannel;
import com.flash3388.flashlib.hal.HalIoInterface;
import com.flash3388.flashlib.robot.io.DigitalOutput;
import com.flash3388.flashlib.robot.io.IoInterface;
import com.hal.Hal;
import com.hal.PortDirection;

public class Main {

    public static void main(String[] args) throws Exception {
        loadNative();

        try (Hal hal = Hal.initialize()) {
            rawTest(hal);

        }
    }

    private static void rawTest(Hal hal) {
        int handle = hal.dioInit(0, PortDirection.OUTPUT);
        try {
            hal.dioSet(handle, true);
        } finally {
            hal.dioFree(handle);
        }

        handle = hal.aioInit(0, PortDirection.OUTPUT);
        try {
            hal.aioSet(handle, 200);
        } finally {
            hal.aioFree(handle);
        }
    }

    private static void flashlibTest(Hal hal) {
        IoInterface ioInterface = new HalIoInterface(hal);
        DigitalOutput output = ioInterface.newDigitalOutput(new HalIoChannel(0));
        output.set(true);
    }

    private static void loadNative() throws Exception {
        Zip zip = JavaSources.containingJar(Hal.class);

        NativeLibraryLoader loader = new TempNativeLibraryLoader();
        NativeLibraryFinder finder = new ArchivedNativeLibraryFinder(zip);

        NativeLibrary nativeLibrary = finder.find("libhaljni");
        loader.load(nativeLibrary);
    }
}
