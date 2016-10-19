#include "UnityNativeAudio.h"

//#define DEBUG_SINE
#define TABLE_SIZE 200
#define RINGBUF_SIZE 8192
struct UserData {
	bool is_initialized = false;
	unsigned long bufferBytes;
	unsigned long bufferBytesIn;
	unsigned int outchannels;
	unsigned int inchannels;

	const char **outports;
	const char *client_name = "Unity3D";
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;

#ifdef DEBUG_SINE
    float sine[200];
    int left_phase;
#endif
    
};

UserData data;

static jack_port_t **outports;
static jack_port_t **inports;


static jack_client_t *client = 0;
static jack_ringbuffer_t *rb;
static jack_ringbuffer_t *rbin;

static unsigned long bufferBytes;

//const int outchannels = 1; // SylR
int process(jack_nframes_t nframes, void *arg) {
	if (!data.is_initialized) {
		std::cout << "running before init" << std::endl;
		return 1;
	}

	int outchannels = data.outchannels; // SylR
	int inchannels = data.inchannels; // SylR

	sample_t *out[MAX_CHANNELS];  // SylR
	sample_t *in[MAX_CHANNELS];  // SylR


	// OUT
	for (int ch = 0; ch < outchannels; ch++){
		out[ch] = (sample_t*)jack_port_get_buffer(outports[ch], nframes);
	}

	// IN
	// Get input ports
	for (int ch = 0; ch < inchannels; ch++)
	{
		in[ch] = (sample_t *)jack_port_get_buffer(inports[ch], nframes);
	}

	
#ifndef DEBUG_SINE

    for (int i = 0; i < nframes; i++)
    {
		// IN
		for (int ch = 0; ch < inchannels; ch++)
		{
            jack_ringbuffer_write(rbin, (char *)in[ch], sizeof(jack_default_audio_sample_t));
			in[ch]++;           
		}
        
		// OUT
		for (int ch = 0; ch < outchannels; ch++)
		{
			jack_ringbuffer_read(rb, (char *)out[ch], sizeof(jack_default_audio_sample_t));
			out[ch]++;
		}
    }

#else
    
    for (int i = 0; i < nframes; i++)
    {
        for (int ch = 0; ch < inchannels; ch++)
        {
            *out[ch] = data.sine[data.left_phase];  /* left */
            data.left_phase += 3;
            if( data.left_phase >= TABLE_SIZE ) data.left_phase -= TABLE_SIZE;
            out[ch]++; // this is actually increasing the count
        }
    }
#endif
    return 0;
}

void jack_shutdown(void *arg)
{
	std::cout << "Jack closing" << std::endl;
}

int startNativeAudio(int inchannels, int outchannels) {

	// stop before
	stopNativeAudio();

    data.inchannels = inchannels;
    data.outchannels = outchannels;
    
	// create new
	client = jack_client_open(data.client_name, data.options, &data.status, data.server_name);
	if (client == NULL) {
		fprintf(stderr, "jack_client_open() failed, status = 0x%2.0x\n", data.status);
		if (data.status & JackServerFailed) {
			fprintf(stderr, "Unable to connect to JACK server\n");
		}
		return -1;
	}
	if (data.status & JackServerStarted) {
		fprintf(stderr, "JACK server started\n");
	}
	if (data.status & JackNameNotUnique) {
		data.client_name = jack_get_client_name(client);
		fprintf(stderr, "unique name `%s' assigned\n", data.client_name);
	}

	/* create the ringbuffers */
	rb = jack_ringbuffer_create(outchannels * sizeof(jack_default_audio_sample_t) * RINGBUF_SIZE);
	rbin = jack_ringbuffer_create(inchannels * sizeof(jack_default_audio_sample_t) * RINGBUF_SIZE);

    // TODO: this might be unnecesary
    //jack_ringbuffer_mlock(rbin);
	/* tell the JACK server to call `process()' whenever
	 there is work to be done.
	 */

	jack_set_process_callback(client, process, 0);

	/* tell the JACK server to call `jack_shutdown()' if
	 it ever shuts down, either entirely, or if it
	 just decides to stop calling us.
	 */

	jack_on_shutdown(client, jack_shutdown, 0);

	/* display the current sample rate.
	 */

#ifdef WIN32
	std::cout << "engine sample rate: " << jack_get_sample_rate(client) << std::endl;
	std::cout << "engine buffer size: " << jack_get_buffer_size(client) << std::endl;
#else
    std::cout << jack_get_sample_rate(client) << std::endl;
    std::cout << jack_get_buffer_size(client) << std::endl;
#endif
	/* create out outports */

	outports = (jack_port_t **)calloc(sizeof(jack_port_t *), outchannels);
	for (int ch = 0; ch < outchannels; ch++){
		char temp[500];
		sprintf(temp, "out_%d", ch + 1);
		outports[ch] = jack_port_register(client, strdup(temp),
			JACK_DEFAULT_AUDIO_TYPE,
			JackPortIsOutput, 0);
	}

    /* create inports */

    inports = (jack_port_t **)calloc(sizeof(jack_port_t *), inchannels); //TODO 2 for now
    for (int ch = 0; ch < inchannels; ch++){
        char temp[500];
        sprintf(temp,"in_%d",ch+1);
        inports[ch] = jack_port_register (client, strdup(temp),
                                           JACK_DEFAULT_AUDIO_TYPE,
                                           JackPortIsInput, 0);
    }
    
    

	// TODO: Anpassen!!!
	if ((outports[0] == NULL) || (outports[1] == NULL)) {
		fprintf(stderr, "no more JACK outports available\n");
		return -1;
	}

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate(client)) {
		stopNativeAudio();
		return -1;
	}

	data.outports = jack_get_ports(client, NULL, NULL,
		JackPortIsPhysical | JackPortIsInput);
	if (data.outports == NULL) {
		fprintf(stderr, "no physical playback outports\n");
		return -1;
	}

	for (int ch = 0; data.outports && data.outports[ch] != NULL && ch < outchannels; ch++){


		if (jack_connect(client, jack_port_name(outports[ch]), data.outports[ch])) {
			fprintf(stderr, "cannot connect output outports\n");
		}

	}

#ifdef DEBUG_SINE
    for( int i=0; i<TABLE_SIZE; i++ )
    {
        data.sine[i] = 0.2 * (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. );
    }
    data.left_phase = 0;
#endif
	/*
	 Save for later

	 if (jack_connect (client, jack_port_name (outports[0]), data.outports[0])) {
	 fprintf (stderr, "cannot connect output outports\n");
	 }

	 if (jack_connect (client, jack_port_name (outports[1]), data.outports[1])) {
	 fprintf (stderr, "cannot connect output outports\n");
	 }
	 */

	//    jack_free(data.outports);

	unsigned int bufferFrames = jack_get_buffer_size(client);
	bufferBytes = bufferFrames * outchannels * sizeof(sample_t);


	data.bufferBytes = bufferBytes;

	data.bufferBytesIn = bufferFrames * inchannels * sizeof(sample_t);;


	data.is_initialized = true;

	std::cout << "Jack Setup finished" << std::endl;

	return 0; // success

}

void stopNativeAudio() {

	if (!data.is_initialized) return;
	printf("stoping native audio");

	jack_client_close(client);
	client = 0;
	bufferBytes = 0;

	jack_ringbuffer_reset(rb);
	jack_ringbuffer_free(rb);
	rb = 0;

	jack_ringbuffer_reset(rbin);
	jack_ringbuffer_free(rbin);
	rbin = 0;

	data.is_initialized = false;
}

int setAudioBuffer(sample_t *buffer) 
{
    if (data.is_initialized) {
        if (jack_ringbuffer_write_space(rb) > 0) jack_ringbuffer_write(rb, (char*)buffer, data.bufferBytes);
    }
        
	return 0;
}

int getAudioBuffer(sample_t *buffer)
{
	if (data.is_initialized) {
		// char tempbuffer[data.bufferBytesIn];
        if (jack_ringbuffer_read_space(rbin) > 0) {
            jack_ringbuffer_read(rbin, (char*)buffer, data.bufferBytesIn);
        } else {
            std::cout << "No samples available" << std::endl;
        }
        
	}
	return 0;
}


const char* getAudioDeviceName(unsigned int deviceId) {
	char* c = jack_get_client_name(client);
	return c;
}

unsigned int getAudioDeviceCount() {
	return 1;
}

unsigned int getDefaultAudioDevice() {
	return 0;
}

unsigned int getAudioInputChannels(unsigned int deviceId) {
	return 2;
}

unsigned int getBufferFrames() {
	return _BUFFER_FRAMES;
}

unsigned int getSampleRate() {
	return _SAMPLE_RATE;
}

void listDevices() {}
