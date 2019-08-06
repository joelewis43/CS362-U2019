package UrlValidatorUnitTest;

import junit.framework.TestCase;

import default2.UrlValidator;

import UrlValidatorUnitTest.ResultPair;

import java.util.Random;






public class UrlValidatorIsValid extends TestCase {

    protected void setUp() throws Exception {
        super.setUp();
    }

    
    protected void tearDown() throws Exception {
        super.tearDown();
    }

    
    public void testIsValid() {
    	
    	for (int i=0; i<100; i++) {
    	
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

            // check the values
            assertEquals(expectation == actual, "Reality is often dissapointing.");
    		
    	}
    }

    
    public ResultPair generateURL() {

        Random rand = new Random();
        
        int falseID = rand.nextInt(5);
        
        ResultPair rp;
        
        
        if (falseID == 0) {
        	//System.out.println("The scheme is false");
        	rp = falseScheme();
        }
        else if (falseID == 1) {
        	//System.out.println("The authority is false");
        	rp = falseAuthority();
        }
        else if (falseID == 2) {
        	//System.out.println("The port is false");
        	rp = falsePort();
        }
        else if (falseID == 3) {
        	//System.out.println("The path is false");
        	rp = falsePath();
        }
        else {
        	//System.out.println("The query is false");
        	rp = falseQuery();
        }
        
        
        return rp;
    }

    
    public ResultPair falseScheme() {
    	
    	Random rand = new Random();
    	String url = "";
    	
    	int scheme = rand.nextInt(testSchemeFalse.length);
    	int authority = rand.nextInt(testAuthority.length);
    	int port = rand.nextInt(testPort.length);
    	int path = rand.nextInt(testPath.length);
    	int query = rand.nextInt(testQuery.length);
    	
    	url += testSchemeFalse[scheme];
    	url += testAuthority[authority];
    	url += testPort[port];
    	url += testPath[path];
    	url += testQuery[query];
    			
    	ResultPair rp = new ResultPair(url, false);
    	
    	return rp;
    	
    }
    
    
    public ResultPair falseAuthority() {
    	
    	Random rand = new Random();
    	String url = "";
    	
    	int scheme = rand.nextInt(testScheme.length);
    	int authority = rand.nextInt(testAuthorityFalse.length);
    	int port = rand.nextInt(testPort.length);
    	int path = rand.nextInt(testPath.length);
    	int query = rand.nextInt(testQuery.length);
    	
    	url += testScheme[scheme];
    	url += testAuthorityFalse[authority];
    	url += testPort[port];
    	url += testPath[path];
    	url += testQuery[query];
    			
    	ResultPair rp = new ResultPair(url, false);
    	
    	return rp;
    	
    }
    
    
    public ResultPair falsePort() {
    	
    	Random rand = new Random();
    	String url = "";
    	
    	int scheme = rand.nextInt(testScheme.length);
    	int authority = rand.nextInt(testAuthority.length);
    	int port = rand.nextInt(testPortFalse.length);
    	int path = rand.nextInt(testPath.length);
    	int query = rand.nextInt(testQuery.length);
    	
    	url += testScheme[scheme];
    	url += testAuthority[authority];
    	url += testPortFalse[port];
    	url += testPath[path];
    	url += testQuery[query];
    			
    	ResultPair rp = new ResultPair(url, false);
    	
    	return rp;
    	
    }
    
    
    public ResultPair falsePath() {
    	
    	Random rand = new Random();
    	String url = "";
    	
    	int scheme = rand.nextInt(testScheme.length);
    	int authority = rand.nextInt(testAuthority.length);
    	int port = rand.nextInt(testPort.length);
    	int path = rand.nextInt(testPathFalse.length);
    	int query = rand.nextInt(testQuery.length);
    	
    	url += testScheme[scheme];
    	url += testAuthority[authority];
    	url += testPort[port];
    	url += testPathFalse[path];
    	url += testQuery[query];
    			
    	ResultPair rp = new ResultPair(url, false);
    	
    	return rp;
    	
    }
    
    
    public ResultPair falseQuery() {
    	
    	Random rand = new Random();
    	String url = "";
    	
    	int scheme = rand.nextInt(testScheme.length);
    	int authority = rand.nextInt(testAuthority.length);
    	int port = rand.nextInt(testPort.length);
    	int path = rand.nextInt(testPath.length);
    	int query = rand.nextInt(testQueryFalse.length);
    	
    	url += testScheme[scheme];
    	url += testAuthority[authority];
    	url += testPort[port];
    	url += testPath[path];
    	url += testQueryFalse[query];
    			
    	ResultPair rp = new ResultPair(url, false);
    	
    	return rp;
    	
    }
    
    
    
    
    
    public void assertEquals(boolean exp, String msg) {
    
    	if (exp) {
    		//System.out.println("Test Passed!");
    	}
    	else {
    		fail(msg);
    	}    	
    }
    
    
    ResultPair[] testScheme = {
        new ResultPair("http://", true),
        new ResultPair("ftp://", true),
        new ResultPair("h3t://", true)
    };

    ResultPair[] testAuthority = {
        new ResultPair("www.google.com", true),
        new ResultPair("www.google.com.", true),
        new ResultPair("go.com", true),
        new ResultPair("go.au", true),
        new ResultPair("0.0.0.0", true),
        new ResultPair("255.255.255.255", true),
        new ResultPair("255.com", true),
        new ResultPair("go.cc", true)
    };
    
    ResultPair[] testPort = {
        new ResultPair(":80", true),
        new ResultPair(":65535", true),
        new ResultPair(":0", true),
        new ResultPair("", true),
    };
    
    ResultPair[] testPath = {
        new ResultPair("/test1", true),
        new ResultPair("/t123", true),
        new ResultPair("/$23", true),
        new ResultPair("/test1/", true),
        new ResultPair("", true),
        new ResultPair("/test1/file", true)
    };
    
    ResultPair[] testQuery = {
        new ResultPair("?action=view", true),
        new ResultPair("?action=edit&mode=up", true),
        new ResultPair("", true)
    };


    
    
    ResultPair[] testSchemeFalse = {
            new ResultPair("3ht://", false),
            new ResultPair("http:/", false),
            new ResultPair("http:", false),
            new ResultPair("http/", false),
            new ResultPair("://", false)
        };

    ResultPair[] testAuthorityFalse = {
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
    
    ResultPair[] testPortFalse = {
        new ResultPair(":65536", false),
        new ResultPair(":-1", false),
        new ResultPair(":65636", false),
        new ResultPair(":999999999999999999", false),
        new ResultPair(":65a", false)
    };
    
    ResultPair[] testPathFalse = {
        new ResultPair("/..", false),
        new ResultPair("/../", false),
        new ResultPair("/..//file", false),
        new ResultPair("/test1//file", false)
    };
    
    ResultPair[] testQueryFalse = {
        new ResultPair("XXX!test=wrong", false)
    };



    
    
    
    
    
    
}