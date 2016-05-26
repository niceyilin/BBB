#!/usr/bin/perl

#print("hello world\n");

$LED_PATH="/sys/class/leds/beaglebone:green:usr";
my $cmd = $ARGV[0];

if ($#ARGV != 0){ # 0 means there's exactly one argument!!!
    print("Error : wrong usage\n"); #die();
    print("./BashLED.sh followed by one of these cmd...\n-on\n-off\n-flash\n-status\n-rotate\ne.g. ./PerlLED.pl on\n");
    exit(2);
}

if ($cmd eq "on") {
    print("Turn LED on now\n");
    setTrigger("none", "led1");
    writeLED3("brightness", 1, "led1");
}
elsif ($cmd eq "off") {
    print("Turn LED off now\n");
    setTrigger("none", "led0");
    writeLED3("brightness", 0, "led0");
}
elsif ($cmd eq "flash") {
    print("Flash LED now\n");
    setTrigger("timer", "led0");
    setTimer(500, "on", "led0");
    setTimer(500, "off", "led0");
}
elsif ($cmd eq "status") {
    print("LED Trigger Status:\n");
    returnTrigger("led0");
}
elsif ($cmd eq "rotate") {
    print("Rotate all the LEDs\n"); 
    setTrigger("timer", "led0");
    setTimer(50,  "on",  "led0");
    setTimer(150, "off", "led0");
    sleep(0.05);
    setTrigger("timer", "led1");
    setTimer(50,  "on",  "led1");
    setTimer(150, "off", "led1");
    sleep(0.05);
    setTrigger("timer", "led2");
    setTimer(50,  "on",  "led2");
    setTimer(150, "off", "led2");
    sleep(0.05);
    setTrigger("timer", "led3");
    setTimer(50,  "on",  "led3");
    setTimer(150, "off", "led3");
}
else {
    die("Error: unknown command.\n\n-on\n-off\n-flash\n-status\ne.g. ./BashLED.sh on\n");   
}

sub writeLED3(){
    my $fileName = shift;
    my $val = shift;
    my $fp;
    my $which_led = shift;
    my $real_path;
    $real_path = getPath($which_led);
    #print "$fileName  $val\n";
    open($fp, "> $real_path/$fileName")
        or die("Can't open file. $!");
    print $fp "$val";
    close($fp);
}

sub setTrigger(){
    my $fp;
    my $val = shift;
    my $which_led = shift;
    my $real_path;
    $real_path = getPath($which_led);
    open($fp, "> $real_path/trigger")  
        or die("Can't open file. $!");
    print $fp "$val";
    close($fp);
}

sub returnTrigger(){
    my $fp;
    my $line;
    my @array;
    my $which_led = shift;
    my $real_path;
    $real_path = getPath($which_led);
    open($fp, "< $real_path/trigger")  
        or die("Can't open file. $!");
    foreach $line (<$fp>){
        @array = split " ", $line;
        foreach $element (@array){
            print ("$element \n");
        }
        #print ("$line");     
    }
    close($fp);
}

sub setTimer(){
    my $fpOn, $fpOff;
    my $val = shift;
    my $on_off = shift;
    my $which_led = shift;
    my $real_path;
    $real_path = getPath($which_led);
    if ($on_off eq "on"){
        open($fpOn, "> $real_path/delay_on")  
            or die("Can't open file. $!");
        print $fpOn "$val";
        close($fpOn);
    }
    else{
        open($fpOff, "> $real_path/delay_off")  
            or die("Can't open file. $!");
        print $fpOff "$val";
        close($fpOff);
    }
}
    
sub getPath(){
    my $which_led = shift;
    my $real_path;
    if ($which_led eq "led0"){
        $real_path = "$LED_PATH"."0";
    }
    elsif ($which_led eq "led1"){
        $real_path = "$LED_PATH"."1";
    }
    elsif ($which_led eq "led2"){
        $real_path = "$LED_PATH"."2";
    }
    else{
        $real_path = "$LED_PATH"."3";
    }
    print "running in ",$real_path,"\n";
    return $real_path;
    
}
