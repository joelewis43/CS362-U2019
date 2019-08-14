package UrlValidatorUnitTest;

import junit.framework.TestCase;

import default2.UrlValidator;

import UrlValidatorUnitTest.ResultPair;

import java.util.Random;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertFalse;



public class Urlrandomtest extends TestCase {

    protected void setUp() throws Exception {
        super.setUp();
    }


    protected void tearDown() throws Exception {
        super.tearDown();
    }
    
    
    public void testPartitionOne() {
    	long options = UrlValidator.ALLOW_ALL_SCHEMES +
                UrlValidator.ALLOW_2_SLASHES;
        UrlValidator urlVal = new UrlValidator(options);
        
        
        assertTrue(urlVal.isValid("http://www.google.com"));
        assertTrue(urlVal.isValid("http://255.255.255.255:80/test1"));
        assertTrue(urlVal.isValid("http://go.com?action=view"));
        
        assertFalse(urlVal.isValid("http://")); //only valid Scheme
        assertFalse(urlVal.isValid("go.com")); //only valid Authority
        assertFalse(urlVal.isValid(":80")); //only valid Port
        assertFalse(urlVal.isValid("/test1")); //only valid Path 
        assertFalse(urlVal.isValid("?action=view")); //only valid Query 
      
    }
    
    public void testPartitionTwo() {
    	long options = UrlValidator.ALLOW_ALL_SCHEMES +
                UrlValidator.ALLOW_2_SLASHES;
        UrlValidator urlVal = new UrlValidator(options);
        
        //Testing Min and Max port
        assertTrue(urlVal.isValid("http://www.google.com:0"));
        assertTrue(urlVal.isValid("http://www.google.com:65535"));
        assertFalse(urlVal.isValid("http://www.google.com:-1"));
        assertFalse(urlVal.isValid("http://www.google.com:65536"));
        
        //Testing Min and Max Authority
        assertTrue(urlVal.isValid("http://0.0.0.0"));
        assertTrue(urlVal.isValid("http://255.255.255.255"));
        assertFalse(urlVal.isValid("http://0.0.0.-1"));
        assertFalse(urlVal.isValid("http://255.255.255.256"));
        
        //Testing null
        assertFalse(urlVal.isValid(null));
        
    }
    


    public void testPartitionThree() {
    	
    	int fails = 0;

    	for (int i=0; i<200; i++) {

    		// allocate a UrlVal object and its options
            long options = UrlValidator.ALLOW_ALL_SCHEMES +
                UrlValidator.ALLOW_2_SLASHES;
            
            UrlValidator urlVal = new UrlValidator(options);

            // generate a URL and its expected validity
            ResultPair rp = generateURL();

            // pull the URL and validity out
            String url = rp.item;
            boolean expectation = rp.valid;

            // test the URL
            boolean actual = urlVal.isValid(url);

            //check the value
            if(expectation == actual) {
            	System.out.println("Passed!");
            }
            else {
                System.out.println("Because of bug, valid URl will be judged to be invalid ");
                System.out.println("Valid URL: " + url);
                System.out.println("Expected result: " + expectation);
                System.out.println("Actual result: " + actual);
                System.out.println("\n");
                fails++;
            }
            
    	}
    	
    	System.out.println("\nThe total number of failures was: " + fails);
    }


    public ResultPair generateURL() {

        String url = "";

        Random rand = new Random();
        
        boolean result = false;

        int scheme = rand.nextInt(testScheme.length);
        int authority = rand.nextInt(testAuthority.length);
        int port = rand.nextInt(testPort.length);
        int path = rand.nextInt(testPath.length);
        int query = rand.nextInt(testQuery.length);
        	
        url += testScheme[scheme].item;
        url += testAuthority[authority].item;
        url += testPort[port].item;
        url += testPath[path].item;
        url += testQuery[query].item;
        
        boolean sch = testScheme[scheme].valid;
        
        boolean auth = testAuthority[authority].valid;
        
        boolean po = testPort[port].valid;
        
        boolean pa = testPath[path].valid;
        
        boolean qu = testQuery[query].valid;
        
        if(sch == false){
        	result = false;
        	System.out.println("scheme is invalid\n");
        }
        else if(auth == false){
        	result = false;
        	System.out.println("authority is invalid\n");
        }
        else if(po == false){
        	result = false;
        	System.out.println("port is invalid\n");
        }
        else if(pa == false){
        	result = false;
        	System.out.println("path is invalid\n");
        }
        else if(qu == false){
        	result = false;
        	System.out.println("query is invalid\n");
        }
        else { 
        	result = true;
        }
        
        
        ResultPair rp = new ResultPair(url, result);

        return rp;
    }


    ResultPair[] testScheme = {
    	new ResultPair("http://", true),
        new ResultPair("ftp://", true),
        new ResultPair("h3t://", true),
        new ResultPair("3ht://", false),
        new ResultPair("http:/", false),
        new ResultPair("http:", false),
        new ResultPair("http/", false),
        new ResultPair("://", false)
    };
    
    ResultPair[] testAuthority = {
		new ResultPair("www.google.com", true),
		new ResultPair("www.google.com.", true),
	    new ResultPair("go.com", true),
		new ResultPair("go.au", true),
		new ResultPair("0.0.0.0", true),
		new ResultPair("255.255.255.255", true),
		new ResultPair("255.com", true),
		new ResultPair("go.cc", true),
        new ResultPair("1.2.3.4.5", false),	            
        new ResultPair("1.2.3.4.", false),
        new ResultPair("1.2.3", false),
        new ResultPair(".1.2.3.4", false),
        new ResultPair("go.a", false),
        new ResultPair("go.a1a", false),
        new ResultPair("go.1aa", false),
        new ResultPair("aaa.", false),
        new ResultPair(".aaa", false),
        new ResultPair("aaa", false),
        new ResultPair("", false)            
    };
        
    ResultPair[] testPort = {
        new ResultPair(":80", true),
        new ResultPair(":65535", true),
        new ResultPair(":0", true),
        new ResultPair("", true),
        new ResultPair(":65536", false),
        new ResultPair(":-1", false),
        new ResultPair(":65636", false),
        new ResultPair(":999999999999999999", false),
        new ResultPair(":65a", false)
    };
        
    ResultPair[] testPath = {
        new ResultPair("/test1", true),
        new ResultPair("/t123", true),
        new ResultPair("/$23", true),
        new ResultPair("/test1/", true),
        new ResultPair("", true),
        new ResultPair("/test1/file", true),
        new ResultPair("/..", false),
        new ResultPair("/../", false),
        new ResultPair("/..//file", false)
    };
        
    ResultPair[] testQuery = {
        new ResultPair("?action=view", true),
        new ResultPair("?action=edit&mode=up", true),
        new ResultPair("", true),
        new ResultPair("\\S", false)
    };
        
     
    

}
