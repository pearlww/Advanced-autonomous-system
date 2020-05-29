function simple_test(num_trials)

     total_score = 0;

     for i = 1:num_trials
          trial_score = objectify();
          if (trial_score ~= 8)
               fprintf('\n\n\n      ~~~~~~~~~MISTAKE MADE~~~~~~~~~');
               pause(5)
          end
          total_score = total_score + trial_score;          
          pause(2);
          clc
     end

     avg_score = total_score / num_trials;

     clc
     disp('************************************************************');
     disp('************************************************************');
     disp('************************************************************');
     mystring = 'Average Score: %d points out of %d possible, or %d percent!\n';
     fprintf(mystring, total_score, 8*num_trials, round(avg_score/8*100))
     disp('************************************************************');
     disp('************************************************************');
     disp('************************************************************');     
end