using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Http;

namespace dotnetTest {
    public class TestClass {
        public static void Main() {
            Console.WriteLine(new TestClass().Gimme(""));
        }
        public string Gimme(string str) {
            HttpClient client = new HttpClient();
            return client.GetStringAsync("https://canihazip.com/s").GetAwaiter().GetResult();
        }
    }
}
