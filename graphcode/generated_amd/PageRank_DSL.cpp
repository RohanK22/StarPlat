#include "PageRank_DSL.h"

void Compute_PR(graph& g,float beta,float delta,int maxIter,
  float* pageRank)
{
  //Getting platforms
  cl_int status;
  cl_platform_id *platforms = NULL;
  cl_uint number_of_platforms;
  status = clGetPlatformIDs(0, NULL, &number_of_platforms);
  platforms = (cl_platform_id *)malloc(number_of_platforms*sizeof(cl_platform_id));
  status = clGetPlatformIDs(number_of_platforms, platforms, NULL);
  printf("Got Platform, status = %d \n", status);

  //Getting Devices present on platform
  cl_device_id *devices= NULL;
  cl_uint number_of_devices;
  status = clGetDeviceIDs(platforms[0],CL_DEVICE_TYPE_GPU, 0, NULL, &number_of_devices);
  devices = (cl_device_id *)malloc(number_of_devices*sizeof(cl_device_id));
  status = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, number_of_devices, devices, NULL);
  printf("Got Devices, status = %d \n", status);

  //Creating context
  cl_context context;
  context = clCreateContext(NULL, number_of_devices, devices, NULL, NULL, &status);

  //Creating command queue
  cl_command_queue command_queue ;
  command_queue = clCreateCommandQueue(context, devices[0], CL_QUEUE_PROFILING_ENABLE , &status);
  printf("command queue created, status = %d\n", status);

  // CSR BEGIN
  int V = g.num_nodes();
  int E = g.num_edges();

  printf("#nodes:%d\n",V);
  printf("#edges:%d\n",E);
  int* edgeLen = g.getEdgeLen();

  int *h_meta;
  int *h_data;
  int *h_src;
  int *h_weight;
  int *h_rev_meta;

  h_meta = (int *)malloc( (V+1)*sizeof(int));
  h_data = (int *)malloc( (E)*sizeof(int));
  h_src = (int *)malloc( (E)*sizeof(int));
  h_weight = (int *)malloc( (E)*sizeof(int));
  h_rev_meta = (int *)malloc( (V+1)*sizeof(int));

  for(int i=0; i<= V; i++) {
    int temp = g.indexofNodes[i];
    h_meta[i] = temp;
    temp = g.rev_indexofNodes[i];
    h_rev_meta[i] = temp;
  }

  for(int i=0; i< E; i++) {
    int temp = g.edgeList[i];
    h_data[i] = temp;
    temp = g.srcList[i];
    h_src[i] = temp;
    temp = edgeLen[i];
    h_weight[i] = temp;
  }


  cl_mem d_meta = clCreateBuffer(context, CL_MEM_READ_WRITE, (1+V)*sizeof(int), NULL, &status);
  cl_mem d_data = clCreateBuffer(context, CL_MEM_READ_WRITE, (E)*sizeof(int), NULL, &status);
  cl_mem d_src = clCreateBuffer(context, CL_MEM_READ_WRITE, (E)*sizeof(int), NULL, &status);
  cl_mem d_weight = clCreateBuffer(context, CL_MEM_READ_WRITE, (E)*sizeof(int), NULL, &status);
  cl_mem d_rev_meta = clCreateBuffer(context, CL_MEM_READ_WRITE, (V+1)*sizeof(int), NULL, &status);
  cl_mem d_modified_next = clCreateBuffer(context, CL_MEM_READ_WRITE, (V)*sizeof(int), NULL, &status);

  status = clEnqueueWriteBuffer(command_queue,   d_meta , CL_TRUE, 0, sizeof(int)*(V+1),   h_meta, 0, NULL, NULL );
  status = clEnqueueWriteBuffer(command_queue,   d_data , CL_TRUE, 0, sizeof(int)*E,   h_data, 0, NULL, NULL );
  status = clEnqueueWriteBuffer(command_queue,    d_src , CL_TRUE, 0, sizeof(int)*E,    h_src, 0, NULL, NULL );
  status = clEnqueueWriteBuffer(command_queue, d_weight , CL_TRUE, 0, sizeof(int)*E, h_weight, 0, NULL, NULL );
  status = clEnqueueWriteBuffer(command_queue, d_rev_meta , CL_TRUE, 0, sizeof(int)*(V+1), h_rev_meta, 0, NULL, NULL );

  // CSR END

  // TIMER START
  cl_event event;
  double kernelTime ;
  double totalTime = 0.0;
  cl_ulong start, end;
  cl_ulong convertToMS = 1e6;

  //DECLAR DEVICE AND HOST vars in params
  cl_mem d_beta = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float), NULL, &status );
   status = clEnqueueWriteBuffer(command_queue,   d_beta , CL_TRUE, 0, sizeof(float), &beta, 0, NULL, NULL );
  cl_mem d_delta = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float), NULL, &status );
   status = clEnqueueWriteBuffer(command_queue,   d_delta , CL_TRUE, 0, sizeof(float), &delta, 0, NULL, NULL );
  cl_mem d_maxIter = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &status );
   status = clEnqueueWriteBuffer(command_queue,   d_maxIter , CL_TRUE, 0, sizeof(int), &maxIter, 0, NULL, NULL );
  cl_mem d_pageRank = clCreateBuffer(context,CL_MEM_READ_WRITE,(V)*sizeof(float),NULL, &status);


  //BEGIN DSL PARSING 
  cl_mem d_num_nodes = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float), NULL, &status);

  float num_nodes = (float)g.num_nodes( ); 

  //Reading kernel file
  FILE* kernelfp = fopen("PageRank_DSL.cl", "rb"); 
  size_t program_size;
  fseek(kernelfp, 0, SEEK_END);
  program_size = ftell(kernelfp);
  rewind(kernelfp);
  char* kernelSource;
  kernelSource = (char *)malloc((program_size+1)* sizeof(char));
  fread(kernelSource, sizeof(char), program_size, kernelfp);
  kernelSource[program_size] = (char)NULL ;
  fclose(kernelfp);

  //Creating program from source(Create and build Program)
  cl_program program = clCreateProgramWithSource(context, 1, (const char **)&kernelSource, NULL, &status);
  printf("Progran created from source, statuc = %d \n", status);
  status = clBuildProgram(program, number_of_devices, devices, " -I ./", NULL, NULL);
  if(status!=CL_SUCCESS){
    printf(" Program building Failed, status = %d \n ",status);
    exit(0);
  }

  //Variable for launch configuration
  size_t global_size, global_size1;
  size_t local_size, local_size1;
  local_size = 128;
  global_size = (V + local_size -1)/ local_size * local_size;
  local_size1 = 1;
  global_size1 = 1;
  // Creating initpageRank_kernel  Kernel
  cl_kernel initpageRank_kernel = clCreateKernel(program, "initpageRank_kernel", &status);

  // Initialization for pageRank variable
  float pageRankValue = (float)1 / num_nodes; 
  status = clSetKernelArg(initpageRank_kernel, 0 , sizeof(cl_mem), (void *)& d_pageRank);
  status = clSetKernelArg(initpageRank_kernel, 1, sizeof(float) , (void*)& pageRankValue);
  status = clSetKernelArg(initpageRank_kernel, 2, sizeof(int), (void*)&V);
  status = clEnqueueNDRangeKernel(command_queue, initpageRank_kernel, 1, NULL, &global_size, &local_size, 0,NULL,&event);

  clWaitForEvents(1,&event);
  status = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
  status = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
  kernelTime = (double)(end-start)/convertToMS;
  totalTime = totalTime+ kernelTime;
  status = clReleaseKernel(initpageRank_kernel);
  cl_mem d_iterCount = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &status);

  int iterCount = 0; 
  cl_mem d_diff = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(float), NULL, &status);

  float diff; 

  do{
    diff = 0.000000;

    //ForAll started here

    status = clEnqueueWriteBuffer(command_queue, d_diff, CL_TRUE, 0, sizeof(float),&diff , 0, NULL, NULL)

    status = clEnqueueWriteBuffer(command_queue, d_delta, CL_TRUE, 0, sizeof(float),&delta , 0, NULL, NULL)

    status = clEnqueueWriteBuffer(command_queue, d_num_nodes, CL_TRUE, 0, sizeof(float),&num_nodes , 0, NULL, NULL)
    cl_kernel Compute_PR = clCreateKernel(program, "Compute_PR" , &status);
     status = clSetKernelArg(Compute_PR, 0, sizeof(int), (void *) &V);
     status = clSetKernelArg(Compute_PR, 1, sizeof(int), (void *) &E);
     status = clSetKernelArg(Compute_PR, 2, sizeof(cl_mem), (void *) &d_meta);
     status = clSetKernelArg(Compute_PR, 3, sizeof(cl_mem), (void *) &d_data);
     status = clSetKernelArg(Compute_PR, 4, sizeof(cl_mem), (void *) &d_src);
     status = clSetKernelArg(Compute_PR, 5, sizeof(cl_mem), (void *) &d_weight);
     status = clSetKernelArg(Compute_PR, 6, sizeof(cl_mem), (void *) &d_rev_meta);
     status = clSetKernelArg(Compute_PR, 7, sizeof(cl_mem), (void *) &d_meta);
     status = clSetKernelArg(Compute_PR, 8, sizeof(float), (void *) &diff);
     status = clSetKernelArg(Compute_PR, 9, sizeof(float), (void *) &delta);
     status = clSetKernelArg(Compute_PR, 10, sizeof(float), (void *) &num_nodes);
     status = clSetKernelArg(Compute_PR, 11, sizeof(cl_mem), (void *) &d_pageRank);
    status = clEnqueueNDRangeKernel(command_queue,Compute_PR, 1,NULL, &global_size, &local_size , 0,NULL,&event);
    clWaitForEvents(1,&event);
    status = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
    status = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
    kernelTime = (double)(end-start)/convertToMS;
    totalTime = totalTime+ kernelTime;

    status = cleReleaseKernel(Compute_PR);
    iterCount++;

  }while((diff > beta) && (iterCount < maxIter));

  //TIMER STOP
  printf("Total Kernel time = %lf ms.\n ", totalTime);

  clEnqueueReadBuffer(command_queue, d_beta , CL_TRUE, 0, sizeof(float)*1, beta, 0, NULL, NULL );
  clEnqueueReadBuffer(command_queue, d_delta , CL_TRUE, 0, sizeof(float)*1, delta, 0, NULL, NULL );
  clEnqueueReadBuffer(command_queue, d_maxIter , CL_TRUE, 0, sizeof(int)*1, maxIter, 0, NULL, NULL );
  clEnqueueReadBuffer(command_queue, d_pageRank , CL_TRUE, 0, sizeof(float)*V, pageRank, 0, NULL, NULL );
  //Release openCL objects
  printf("Started releasing Objects\n");
  status = clReleaseMemObject(d_meta);
  status = clReleaseMemObject(d_data);
  status = clReleaseMemObject(d_src);
  status = clReleaseMemObject(d_weight);
  status = clReleaseMemObject(d_rev_meta);
  status = clReleaseMemObject(d_modified_next);
  status = clFlush(command_queue);
  status = clFinish(command_queue);
  status = clReleaseCommandQueue(command_queue);
  status = clReleaseContext(context);
  free(h_meta);
  free(h_data);
  free(h_src);
  free(h_weight);
  free(h_rev_meta);
} //end FUN
