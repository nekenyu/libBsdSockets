#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

int main(int argc, char**argv) {
  CPPUNIT_NS::TextUi::TestRunner runner;
  CPPUNIT_NS::TestFactoryRegistry &registry = CPPUNIT_NS::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool wasSuccessful = runner.run();
  return wasSuccessful ? 0 : 1;
}
