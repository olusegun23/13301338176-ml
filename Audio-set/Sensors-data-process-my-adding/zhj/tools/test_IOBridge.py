from unittest import TestCase
from IOBridge import IOBridge

class TestIOBridge(TestCase):


    def test_demo(self):
        s = IOBridge()
        self.assertRaises(Exception, s.demo, 2, 1, 2)
        value=1001
        self.assertEquals(100, value)


